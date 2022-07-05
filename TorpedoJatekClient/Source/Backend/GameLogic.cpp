#include "GameLogic.h"

GameLogic::GameLogic(Fleet& player, Fleet& enemy, Sea& sea)
{
	std::cout << "-----------------------------------------------" << std::endl
		<< output << std::endl
		<< "-----------------------------------------------" << std::endl;

	pMyFleet = &player;
	pEnemyFleet = &enemy;
	pSea = &sea;
}

GameLogic::~GameLogic()
{
	delete clientHandler;
}

//Inicializ�lja a j�t�kkal kapcsolatos h�tt�radatokat
void GameLogic::Init()
{
	if (!TorpedoGLOBAL::Debug) {
		//Lekezeli ha teli szerverre kapcsol�dtunk
		do {
			ConnectionSetup();
			playerNum = clientHandler->GetPlayerNum();
		} while (playerNum > 2);

		mapSize = clientHandler->GetMapSize();
	}

	pSea->Init(mapSize, playZoneCenterOffset);

	pMyFleet->Init(mapSize, playZoneCenterOffset);
	pMyFleet->InitTiles(pSea->getTiles(pMyFleet->getIsAlly()));

	pEnemyFleet->Init(mapSize, playZoneCenterOffset);
	pEnemyFleet->InitTiles(pSea->getTiles(pEnemyFleet->getIsAlly()));

	if (TorpedoGLOBAL::Debug) {
		std::cout << "Disclamer: You are currently running the DEBUG version of the game,\nwhich is only good for graphics testing." << std::endl;
		PlaceShipsINDEBUG();
		SetTilesINDEBUG();
	}
	else {
		unplacedShips = *pMyFleet->getUnplacedShipCount();
	}
}

//Kapcsolatot l�tes�t egy szerverrel
void GameLogic::ConnectionSetup()
{
	do {
		std::cout << "Establishing connection." << std::endl;
		std::cout << "Server ip: ";
		std::cin >> ip;
		std::cout << "Server port: ";
		std::cin >> port;
	} while (!clientHandler->Init(ip, port));
}

//�zenet kiir�sa
void GameLogic::DisplayMessage(GameState gameState, int related_data)
{
	if (gameState == GameState::INITIAL){
		output = "Place your ships!\nChoose what kind of ship do you want to place down.\nPress the length number on your keyboard while the game window is focused.(1-4)";

		std::cout << '\n' <<
			output << '\n'
			<< "1. - 1tile ships left: " << unplacedShips.at(0) << '\n'
			<< "2. - 2tile ships left: " << unplacedShips.at(1) << '\n'
			<< "3. - 3tile ships left: " << unplacedShips.at(2) << '\n'
			<< "4. - 4tile ships left: " << unplacedShips.at(3) << '\n'
			<< "ESC - Quit game! \n" << std::endl;
	}
	else if (gameState == GameState::PLACING_SHIP) {
		if (related_data == 1) {
			std::cout << "Place your ship on a free tile on your side of the map!(with LeftMouseButton)" << std::endl;
		}
		else {
			std::cout << "Place the front of your ship on a free tile on your side of the map!(with LeftMouseButton)" << std::endl;
		}
	}
	else if (gameState == GameState::STARTING_MATCH) {
		std::cout << "Waiting for server to start the match..." <<
			"\n(ESC - Quit)" << std::endl;
	}
	else if (gameState == GameState::SHOOTING_AT_ENEMY) {
		std::cout << "\nIt's your turn! Shoot at an enemy tile!(with LeftMouseButton)" <<
			"\n(ESC - Quit)" << std::endl;
	}
	else if (gameState == GameState::GETTING_SHOT) {
		std::cout << "\nWaiting for enemy to shoot..." <<
			"\n(ESC - Quit)" << std::endl;
	}
	else if (gameState == GameState::MATCH_ENDING) {
		if (related_data == playerNum) {
			pEnemyFleet->getBattleShip().setDestroyed(true);
			std::cout << "You've won the match!\n(ESC-Quit)" << std::endl;
		}
		else{
			pMyFleet->getBattleShip().setDestroyed(true);
			std::cout << "You've lost the match!\n(ESC-Quit)" << std::endl;
		}
	}
}

//Hiba kiir�sa
void GameLogic::DisplayError(GameState gameState, int related_data) 
{
	if (gameState == GameState::SHIP_SIZE_INPUT) {
		std::cout << "You can't place down any more ships of " << related_data
			<< " size!" << std::endl;
	}
}

//Megn�zi, hogy le kell-e rakni m�g egy adott m�ret� haj�t
bool GameLogic::CheckForUnplacedShips(int shipSize)
{
	if (unplacedShips.at(shipSize - 1) > 0) {
		return true;
	}
	return false;
}

//Le kell-e m�g rakni b�rmekkora haj�t
bool GameLogic::CheckAnyUnplacedShipLeft()
{
	return std::any_of(unplacedShips.cbegin(), unplacedShips.cend(), [](int i) {return i != 0; });
}

//Lerak egy haj�t ha �res a kijel�lt mez�
bool GameLogic::PlaceAllyShip(int tile_id, int shipSize)
{

	int offset = pSea->getAlphaOffset();
	int tileIndex = tile_id - offset;
	if (tileIndex < mapSize * mapSize && tileIndex >= 0) {
		//Haj� eleje
		if (!shipFrontPlaced) {
			pShipFront = &pSea->getTileByIndex(tileIndex);
			if (pMyFleet->CheckTile(*pShipFront)) {
				//Ha 1 m�ret�
				if (shipSize == 1) {
					pMyFleet->PlaceShip(pShipFront, NULL);
					std::cout << "--------\n"
						"Ship placed at " << pShipFront->getPos().first << pShipFront->getPos().second <<
						"\n--------" << std::endl;
					--unplacedShips.at(shipSize - 1);
					return true;
				}
				//Ha Nagyobb m�ret�, �sszegy�jti a lehets�ges haj�h�takat
				else {
					freeChoices = pMyFleet->getFreeBacks(*pShipFront, shipSize - 1);
					if (std::none_of(freeChoices.cbegin(), freeChoices.cend(), [](PlayTile* ptr) {return ptr; }))
					{
						std::cout << "No position available for the back of the ship!\n Try another position." << std::endl;
						return false;
					}
					else {
						pShipFront->setState(7); //aqua recoloring
						std::cout << "--------\n"
							"Ship Front location saved at " << pShipFront->getPos().first << pShipFront->getPos().second <<
							"\n--------" << std::endl;
						std::cout << "Select one of the possible locations for the back of the ship!(marked with green)" << std::endl;
						for (PlayTile* &choice : freeChoices) {
							if (choice) {
								choice->setState(6); //green recoloring
								std::cout << choice->getPos().first << choice->getPos().second << std::endl;
							}
						}
						shipFrontPlaced = true;
						return false;
					}

				}
			}
			else {
				std::cout << "Tile is not empty!" << std::endl;
			}
		}
		//Haj� h�ta
		else {
			pShipBack = &pSea->getTileByIndex(tileIndex);
			bool foundInputInChoices = false;
			for (PlayTile* &choisz : freeChoices) {
				if (choisz && choisz->getIndex() == pShipBack->getIndex()) {
					foundInputInChoices = true;
					break;
				}
			}
			//Ha j� helyre kattintunk
			if (foundInputInChoices) {
				pMyFleet->PlaceShip(pShipFront, pShipBack);
				std::cout << "--------\n"
					"Ship placed at " << pShipFront->getPos().first << pShipFront->getPos().second <<
					"/" << pShipBack->getPos().first << pShipBack->getPos().second <<
					"\n--------" << std::endl;
				--unplacedShips.at(shipSize - 1);

				pShipFront->setState(3); //default recoloring
				for (PlayTile* &choice : freeChoices) {
					if (choice) {
						choice->setState(3); //default recoloring
						choice = nullptr;
					}
				}
				shipFrontPlaced = false;
				return true;
			}
		}
	}
	return false;
}

//Elk�ldi a lerakott haj�ink poz�ci�it
void GameLogic::SendFleetToServer()
{
	clientHandler->SendFleet(pMyFleet->getActiveTilePositions());
}

//Megn�zi hogy a szerver el akarja-e ind�tani a j�t�kot
bool GameLogic::CheckStartSignal() 
{
	return clientHandler->GetStartSignal();
}

//Bek�ri a j�t�kost�l,hogy hova akar l�ni,majd k�ldi a szervernek
PlayTile* GameLogic::Shoot(int tile_id)
{
	PlayTile *target;
	
	int offset = pSea->getAlphaOffset();
	int enemyOffset = pSea->getEnemyIndexOffset();
	int tileIndex = tile_id - enemyOffset - offset;
	if (tileIndex < mapSize * mapSize && tileIndex >= 0) {
		target = &pSea->getTileByIndex(tileIndex, false);
		matchState = clientHandler->SendShot(target->getPos());
		target->setState(static_cast<int>(matchState));

		const std::string shootPos = ProcessTile(target->getPos());
		std::cout << "Your shot to " << shootPos << " was a "
			<< (matchState == ResponseState::CONTINUE_MATCH ? "miss." : (matchState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "banger!!")) << std::endl;
		return target;
	}

	return nullptr;
}

//Kapunk egy l�v�st az ellenf�lt�l
PlayTile* GameLogic::GetShoot()
{
	if (clientHandler->CheckForResponse()) {
		std::string shootPos;
		const std::pair<char,int> shootCoord = clientHandler->ReceiveShot();
		matchState = clientHandler->getRecShotState();

		if (shootCoord.first == '0') {
			std::cout << "The enemy left the game!" << std::endl;
		}
		else {
			PlayTile* pTarget = &pMyFleet->getTile(shootCoord);
			shootPos = ProcessTile(pTarget->getPos());

			//Megn�zi hogy van-e m�g lebeg� haj�nk
			if (matchState != ResponseState::CONTINUE_MATCH) {
				pMyFleet->HitFleet(pTarget->getPos());
			}
			pTarget->setState(static_cast<int>(matchState));
			std::cout << "Enemy's shot to " << shootPos << " was a "
				<< (matchState == ResponseState::CONTINUE_MATCH ? "miss." : (matchState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "banger!!")) << std::endl;
			return pTarget;
		}
	}
	return nullptr;
}

//L�v�sek ut�n megn�zi,hogy a szerver jelezte-e,hogy nyert valaki
int GameLogic::CheckVictoryState()
{
	if (matchState == ResponseState::WIN_PLAYER_ONE) {
		clientHandler->CloseConnection();
		return 1;
	}
	else if (matchState == ResponseState::WIN_PLAYER_TWO) {
		clientHandler->CloseConnection();
		return 2;
	}
	return 0;
}

//J�t�kmez� koordin�t�kb�l csin�l sz�veges form�j�t
const std::string GameLogic::ProcessTile(const std::pair<char, int> &tile)
{
	char rowC[10];
	_itoa_s(tile.second, rowC, 10);

	std::string result;
	result.push_back(tile.first);
	result.push_back(rowC[0]);

	return result;
}

//Ha a j�t�k DEBUG m�dba ind�tjuk,akkor be�getetten lerak nek�nk n�h�ny haj�t
void GameLogic::PlaceShipsINDEBUG() 
{
	pMyFleet->PlaceShip(&pMyFleet->getTile(std::pair<char, int>('a', 1)), nullptr);
	pMyFleet->PlaceShip(&pMyFleet->getTile(std::pair<char, int>('g', 5)),
		&pMyFleet->getTile(std::pair<char, int>('g', 7)));
	pMyFleet->PlaceShip(&pMyFleet->getTile(std::pair<char, int>('d', 4)),
		&pMyFleet->getTile(std::pair<char, int>('d', 6)));

	pEnemyFleet->PlaceShip(&pEnemyFleet->getTile(std::pair<char, int>('a', 1)), nullptr);
	pEnemyFleet->PlaceShip(&pEnemyFleet->getTile(std::pair<char, int>('c', 2)),
		&pEnemyFleet->getTile(std::pair<char, int>('c', 3)));
}

//Ha a j�t�k DEBUG m�dba ind�tjuk,akkor be�getetten �talak�t n�h�ny j�t�kmez� �llapotot
void GameLogic::SetTilesINDEBUG() 
{
	pSea->getTileByIndex(0, true).setState(1);
	pSea->getTileByIndex(1, true).setState(2);
	pSea->getTileByIndex(11, true).setState(1);
	pSea->getTileByIndex(14, true).setState(1);
	pSea->getTileByIndex(7, true).setState(1);

	pSea->getTileByIndex(0, false).setState(1);
	pSea->getTileByIndex(1, false).setState(2);
}

//Lez�rja a kapcsolatot a szerverrel
void GameLogic::QuitGame() {
	clientHandler->QuitGame();
}

//J�t�kossz�m
int GameLogic::getPlayerNum()
{
	return playerNum;
}