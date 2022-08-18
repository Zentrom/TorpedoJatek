#include "GameLogic.h"

GameLogic::GameLogic(Fleet& player, Fleet& enemy, Sea& sea, TextHandler& text_handler)
{
	std::cout << "-----------------------------------------------" << std::endl
		<< output << std::endl
		<< "-----------------------------------------------" << std::endl;

	pMyFleet = &player;
	pEnemyFleet = &enemy;
	pSea = &sea;
	pTextHandler = &text_handler;
}

GameLogic::~GameLogic()
{
	delete clientHandler;
}

//Inicializálja a játékkal kapcsolatos háttéradatokat
void GameLogic::Init(std::string ip_num, std::string port_num)
{
	if (!TorpedoGLOBAL::Debug) {
		ip = ip_num;
		port = std::stoi(port_num);

		//Lekezeli ha teli szerverre kapcsolódtunk
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
		if (pTextHandler) {
			*pTextHandler < u8"Disclamer: You are currently running the DEBUG version of the game,\nwhich is only good for graphics testing.\n \
Extra controls: F - Fire | G - Sink";
		}
		PlaceShipsINDEBUG();
		SetTilesINDEBUG();
	}
	else {
		unplacedShips = *pMyFleet->getUnplacedShipCount();
	}
}

//Kapcsolatot létesít egy szerverrel
void GameLogic::ConnectionSetup()
{
	std::cout << "Establishing connection." << std::endl;
	std::cout << "Server ip: " << ip << std::endl;
	std::cout << "Server port: " << port << std::endl;

	clientHandler->Init(ip, port);
}

//Üzenet kiirása
void GameLogic::DisplayMessage(GameState gameState, int related_data)
{
	if (gameState == GameState::INITIAL){
		output = "Place your ships!\nChoose what kind of ship do you want to place down.\nPress the length number on your keyboard while the game window is focused.(1-4)";
		*pTextHandler << u8"Place your ships! Choose what kind of ship do you want to place down.\nPress the length number on your keyboard while the game window is focused.(1-4)";
		
		std::cout << '\n' <<
			output << '\n'
			<< "1. - 1tile ships left: " << unplacedShips.at(0) << '\n'
			<< "2. - 2tile ships left: " << unplacedShips.at(1) << '\n'
			<< "3. - 3tile ships left: " << unplacedShips.at(2) << '\n'
			<< "4. - 4tile ships left: " << unplacedShips.at(3) << '\n'
			<< "ESC - Quit game! \n" << std::endl;
		*pTextHandler
			<< u8"\n1. - 1tile ships left: " << unplacedShips.at(0)
			<< u8"\n2. - 2tile ships left: " << unplacedShips.at(1)
			<< u8"\n3. - 3tile ships left: " << unplacedShips.at(2)
			<< u8"\n4. - 4tile ships left: " << unplacedShips.at(3)
			<< u8"\nESC - Quit game!";
	}
	else if (gameState == GameState::PLACING_SHIP) {
		if (related_data == 1) {
			std::cout << "Place your ship on a free tile on your side of the map!(with LeftMouseButton)" << std::endl;
			*pTextHandler < "Place your ship on a free tile on your side of the map!(with LeftMouseButton)";
		}
		else {
			std::cout << "Place the front of your ship on a free tile on your side of the map!(with LeftMouseButton)" << std::endl;
			*pTextHandler < "Place the front of your ship on a free tile on your side of the map!(with LeftMouseButton)";
		}
	}
	else if (gameState == GameState::STARTING_MATCH) {
		std::cout << "Waiting for server to start the match..." <<
			"\n(ESC - Quit)" << std::endl;
		*pTextHandler << "Waiting for server to start the match...";
		*pTextHandler << "\n(ESC - Quit)";
	}
	else if (gameState == GameState::SHOOTING_AT_ENEMY) {

		if (pEnemyTarget) {
			const std::string shootPos = ProcessTile(pEnemyTarget->getPos());
			pEnemyTarget->setStateColor();
			std::cout << "Enemy's shot to " << shootPos << " was a "
				<< (matchState == ResponseState::CONTINUE_MATCH ? "miss." : (matchState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "game decider!!")) << std::endl;
			(*pTextHandler < "Enemy's shot to ") << shootPos.c_str() << " was a "
				<< (matchState == ResponseState::CONTINUE_MATCH ? "miss." : (matchState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "game decider!!")) << '\n';
		}
		std::cout << "\nIt's your turn! Shoot at an enemy tile!(with LeftMouseButton)" <<
			"\n(ESC - Quit)" << std::endl;
		*pTextHandler << "It's your turn! Shoot at an enemy tile!(with LeftMouseButton)" <<
			"\n(ESC - Quit)";
	}
	else if (gameState == GameState::GETTING_SHOT) {

		if (pMyTarget) {
			const std::string shootPos = ProcessTile(pMyTarget->getPos());
			pMyTarget->setStateColor();
			std::cout << "Your shot to " << shootPos << " was a "
				<< (matchState == ResponseState::CONTINUE_MATCH ? "miss." : (matchState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "game decider!!")) << std::endl;
			(*pTextHandler < "Your shot to ") << shootPos.c_str() << " was a "
				<< (matchState == ResponseState::CONTINUE_MATCH ? "miss." : (matchState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "game decider!!")) << '\n';
		}
		std::cout << "\nWaiting for enemy to shoot..." <<
			"\n(ESC - Quit)" << std::endl;
		*pTextHandler << "Waiting for enemy to shoot..." <<
			"\n(ESC - Quit)";
	}
	else if (gameState == GameState::MATCH_ENDING) {
		if (related_data == playerNum) {
			if (pMyTarget) {
				const std::string shootPos = ProcessTile(pMyTarget->getPos());
				pMyTarget->setStateColor();
				std::cout << "Your shot to " << shootPos << " was a "
					<< (matchState == ResponseState::CONTINUE_MATCH ? "miss." : (matchState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "game decider!!")) << std::endl;
				(*pTextHandler < "Your shot to ") << shootPos.c_str() << " was a "
					<< (matchState == ResponseState::CONTINUE_MATCH ? "miss." : (matchState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "game decider!!"));
			}
			pEnemyFleet->getBattleShip().setDestroyed(true);
			std::cout << "You've won the match!\n(ESC-Quit)" << std::endl;
			*pTextHandler << "\nYou've won the match!\n(ESC-Quit)";
		}
		else{
			if (pEnemyTarget) {
				const std::string shootPos = ProcessTile(pEnemyTarget->getPos());
				pEnemyTarget->setStateColor();
				std::cout << "Enemy's shot to " << shootPos << " was a "
					<< (matchState == ResponseState::CONTINUE_MATCH ? "miss." : (matchState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "game decider!!")) << std::endl;
				(*pTextHandler < "Enemy's shot to ") << shootPos.c_str() << " was a "
					<< (matchState == ResponseState::CONTINUE_MATCH ? "miss." : (matchState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "game decider!!"));
				//Vesztés esetén kirajzoljuk a nyertes hajóit
				PlaceShipsIfLost();
			}

			pMyFleet->getBattleShip().setDestroyed(true);
			std::cout << "You've lost the match!\n(ESC-Quit)" << std::endl;
			*pTextHandler << "\nYou've lost the match!\n(ESC-Quit)";
		}
		clientHandler->CloseConnection();
	}
}

//Hiba kiirása
void GameLogic::DisplayError(GameState gameState, int related_data) 
{
	if (gameState == GameState::SHIP_SIZE_INPUT) {
		std::cout << "You can't place down any more ships of " << related_data
			<< " size!" << std::endl;
		(*pTextHandler < "You can't place down any more ships of ") << related_data
			<< " size!\n";
	}
}

//Megnézi, hogy le kell-e rakni még egy adott méretû hajót
bool GameLogic::CheckForUnplacedShips(int shipSize)
{
	if (unplacedShips.at(shipSize - 1) > 0) {
		return true;
	}
	return false;
}

//Le kell-e még rakni bármekkora hajót
bool GameLogic::CheckAnyUnplacedShipLeft()
{
	return std::any_of(unplacedShips.cbegin(), unplacedShips.cend(), [](int i) {return i != 0; });
}

//Lerak egy hajót ha üres a kijelölt mezõ
bool GameLogic::PlaceAllyShip(int tile_id, int shipSize)
{

	int offset = pSea->getAlphaOffset();
	int tileIndex = tile_id - offset;
	if (tileIndex < mapSize * mapSize && tileIndex >= 0) {
		//Hajó eleje
		if (!shipFrontPlaced) {
			pShipFront = &pSea->getTileByIndex(tileIndex);
			if (pMyFleet->CheckTile(*pShipFront)) {
				//Ha 1 méretû
				if (shipSize == 1) {
					pMyFleet->PlaceShip(pShipFront, NULL);
					std::cout << "--------\n"
						"Ship placed at " << pShipFront->getPos().first << pShipFront->getPos().second <<
						"\n--------" << std::endl;
					(*pTextHandler < "Ship placed at ") << pShipFront->getPos().first << pShipFront->getPos().second <<
						"\n";
					--unplacedShips.at(shipSize - 1);
					return true;
				}
				//Ha Nagyobb méretû, összegyûjti a lehetséges hajóhátakat
				else {
					freeChoices = pMyFleet->getFreeBacks(*pShipFront, shipSize - 1);
					if (std::none_of(freeChoices.cbegin(), freeChoices.cend(), [](PlayTile* ptr) {return ptr; }))
					{
						std::cout << "No position available for the back of the ship!\n Try another position." << std::endl;
						*pTextHandler < "No position available for the back of the ship!\n Try another position.";
						return false;
					}
					else {
						pShipFront->setState(7); //aqua recoloring
						std::cout << "--------\n"
							"Ship Front location saved at " << pShipFront->getPos().first << pShipFront->getPos().second <<
							"\n--------" << std::endl;
						(*pTextHandler < "Ship Front location saved at ") << pShipFront->getPos().first << pShipFront->getPos().second <<
							"\n";
						std::cout << "Select one of the possible locations for the back of the ship!(marked with green)" << std::endl;
						*pTextHandler << "Select one of the possible locations for the back of the ship!(marked with green)";
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
				*pTextHandler < "Tile is not empty!";
			}
		}
		//Hajó háta
		else {
			pShipBack = &pSea->getTileByIndex(tileIndex);
			bool foundInputInChoices = false;
			for (PlayTile* &choisz : freeChoices) {
				if (choisz && choisz->getIndex() == pShipBack->getIndex()) {
					foundInputInChoices = true;
					break;
				}
			}
			//Ha jó helyre kattintunk
			if (foundInputInChoices) {
				pMyFleet->PlaceShip(pShipFront, pShipBack);
				std::cout << "--------\n"
					"Ship placed at " << pShipFront->getPos().first << pShipFront->getPos().second <<
					"/" << pShipBack->getPos().first << pShipBack->getPos().second <<
					"\n--------" << std::endl;
				(*pTextHandler < "Ship placed at ") << pShipFront->getPos().first << pShipFront->getPos().second <<
					"/" << pShipBack->getPos().first << pShipBack->getPos().second << "\n";
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

//Elküldi a lerakott hajóink pozícióit
void GameLogic::SendFleetToServer()
{
	clientHandler->SendFleet(pMyFleet->getShipPositions());
}

//Megnézi hogy a szerver el akarja-e indítani a játékot
bool GameLogic::CheckStartSignal() 
{
	return clientHandler->GetStartSignal();
}

//Bekéri a játékostól,hogy hova akar lõni,majd küldi a szervernek
PlayTile* GameLogic::Shoot(int tile_id)
{
	int offset = pSea->getAlphaOffset();
	int enemyOffset = pSea->getEnemyIndexOffset();
	int tileIndex = tile_id - enemyOffset - offset;
	if (tileIndex < mapSize * mapSize && tileIndex >= 0) {
		pMyTarget = &pSea->getTileByIndex(tileIndex, false);
		matchState = clientHandler->SendShot(pMyTarget->getPos());
		pMyTarget->setState(static_cast<int>(matchState));
		return pMyTarget;
	}

	return nullptr;
}

//Kapunk egy lövést az ellenféltõl
PlayTile* GameLogic::GetShoot()
{
	if (clientHandler->CheckForResponse()) {
		
		const std::pair<char,int> shootCoord = clientHandler->ReceiveShot();
		matchState = clientHandler->getRecShotState();

		if (shootCoord.first == '0') {
			std::cout << "The enemy left the game!" << std::endl;
			//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Info", "The enemy left the game!", nullptr);
		}
		else {
			pEnemyTarget = &pMyFleet->getTile(shootCoord);
			pEnemyTarget->setState(static_cast<int>(matchState));
			//Megnézi hogy van-e még lebegõ hajónk
			if (matchState != ResponseState::CONTINUE_MATCH) {
				pMyFleet->HitFleet(pEnemyTarget->getPos());
			}
			return pEnemyTarget;
		}
	}
	return nullptr;
}

//Lövések után megnézi,hogy a szerver jelezte-e,hogy nyert valaki
int GameLogic::CheckVictoryState()
{
	if (matchState == ResponseState::WIN_PLAYER_ONE) {
		return 1;
	}
	else if (matchState == ResponseState::WIN_PLAYER_TWO) {
		return 2;
	}
	return 0;
}

//Játékmezõ koordinátákból csinál szöveges formájút
const std::string GameLogic::ProcessTile(const std::pair<char, int> &tile)
{
	char rowC[10];
	_itoa_s(tile.second, rowC, 10);

	std::string result;
	result.push_back(tile.first);
	result.push_back(rowC[0]);

	return result;
}

//Ha vesztettünk, kirajzoljuk az ellenfél hajóit
void GameLogic::PlaceShipsIfLost()
{
	std::vector<std::vector<std::pair<char, int>>> winnerShips = clientHandler->GetEnemyShipsIfLost();
	PlayTile* pFront;
	PlayTile* pBack = nullptr;
	for (std::vector<std::pair<char, int>> &sh : winnerShips) {
		pFront = &pEnemyFleet->getTile(sh.front());
		if (sh.size() > 1) {
			pBack = &pEnemyFleet->getTile(sh.back());
		}
		pEnemyFleet->PlaceShip(pFront, pBack);
	
		pBack = nullptr;
	}
}

//Ha a játék DEBUG módba indítjuk,akkor beégetetten lerak nekünk néhány hajót
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

//Ha a játék DEBUG módba indítjuk,akkor beégetetten átalakít néhány játékmezõ állapotot
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

//Lezárja a kapcsolatot a szerverrel
void GameLogic::QuitGame() {
	clientHandler->QuitGame();
}

//Játékosszám
int GameLogic::getPlayerNum()
{
	return playerNum;
}