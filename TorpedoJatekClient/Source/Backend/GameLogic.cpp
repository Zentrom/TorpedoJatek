#include "GameLogic.h"

GameLogic::GameLogic(void)
{
}

GameLogic::~GameLogic(void)
{
}

//Inicializálja a háttérlogikát
int GameLogic::Init(Fleet*& player, Fleet*& enemy, Sea*& sea)
{
	std::cout << "-----------------------------------------------" << std::endl
		<< output << std::endl
		<< "-----------------------------------------------" << std::endl
		<< std::endl;

	myFleet = player;
	enemyFleet = enemy;
	mySea = sea;

	if (!TorpedoGLOBAL::Debug) {
		ConnectionSetup();
		playerNum = clientHandler.GetPlayerNum();
		mapSize = clientHandler.GetMapSize();
	}

	return mapSize;
}

//Kapcsolatot létesít egy szerverrel
void GameLogic::ConnectionSetup()
{
	do {
		std::cout << "Establishing connection." << std::endl;
		std::cout << "Server ip: ";
		std::cin >> ip;
		std::cout << "Server port: ";
		std::cin >> port;
	} while (!clientHandler.Init(ip, port));
}

//Inicializálja a játékkal kapcsolatos háttéradatokat
void GameLogic::InitGame()
{
	mySea->Init(mapSize);

	myFleet->Init(mapSize, true);
	myFleet->InitTiles(mySea->getTiles(true));

	enemyFleet->Init(mapSize, false);
	enemyFleet->InitTiles(mySea->getTiles(false));

	if (TorpedoGLOBAL::Debug) {
		std::cout << "Disclamer: You are currently running the DEBUG version of the game,\nwhich is only good for graphics testing." << std::endl;
		PlaceShipsINDEBUG();
		SetTilesINDEBUG();
	}
	else {
		unplacedShips = myFleet->getUnplacedShipCount();
	}
}

//Üzenet kiirása
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
		//int* shipSizePointer = static_cast<int*>(relatedData);
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
		//int* winnerNum = static_cast<int*>(relatedData);
		if (//*winnerNum 
			related_data == playerNum) {
			enemyFleet->getBattleShip().setDestroyed(true);
			std::cout << "You've won the match!\n(ESC-Quit)" << std::endl;
		}
		else{
			myFleet->getBattleShip().setDestroyed(true);
			std::cout << "You've lost the match!\n(ESC-Quit)" << std::endl;
		}
	}
}

//Hiba kiirása
void GameLogic::DisplayError(GameState gameState, int related_data) 
{
	if (gameState == GameState::SHIP_SIZE_INPUT) {
		//int* shipSizePointer = static_cast<int*>(relatedData);
		std::cout << "You can't place down any more ships of " << related_data//<< *shipSizePointer 
			<< " size!" << std::endl;
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
bool GameLogic::PlaceShip(int tileIndex, int shipSize)
{
	int offset = mySea->getTileByIndex(0).getIndexOffset();
	if (tileIndex - offset < mapSize * mapSize && tileIndex - offset >= 0) {
		if (!shipFrontPlaced) {
			shipFront = &mySea->getTileByIndex(tileIndex - offset);
			if (myFleet->CheckTile(*shipFront)) {
				if (shipSize == 1) {
					myFleet->PlaceShip(shipFront, NULL);
					std::cout << "--------\n"
						"Ship placed at " << shipFront->getPos().first << shipFront->getPos().second <<
						"\n--------" << std::endl;
					--unplacedShips.at(shipSize - 1);
					return true;
				}
				else {
					freeChoices = myFleet->getFreeBacks(*shipFront, shipSize - 1);
					if (std::none_of(freeChoices.cbegin(), freeChoices.cend(), [](PlayTile* ptr) {return ptr; }))
					{
						std::cout << "No position available for the back of the ship!\n Try another position." << std::endl;
						return false;
					}
					else {
						shipFront->setState(7); //aqua recoloring
						std::cout << "--------\n"
							"Ship Front location saved at " << shipFront->getPos().first << shipFront->getPos().second <<
							"\n--------" << std::endl;
						std::cout << "Select one of the possible locations for the back of the ship!(marked with green)" << std::endl;
						for (PlayTile* choice : freeChoices) {
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
		else {
			shipBack = &mySea->getTileByIndex(tileIndex - offset);
			bool foundInputInChoices = false;
			for (PlayTile* choisz : freeChoices) {
				if (choisz && choisz->getIndex() == shipBack->getIndex()) {
					foundInputInChoices = true;
					break;
				}
			}
			if (foundInputInChoices) {
				myFleet->PlaceShip(shipFront, shipBack);
				std::cout << "--------\n"
					"Ship placed at " << shipFront->getPos().first << shipFront->getPos().second <<
					"/" << shipBack->getPos().first << shipBack->getPos().second <<
					"\n--------" << std::endl;
				--unplacedShips.at(shipSize - 1);

				shipFront->setState(3); //default recoloring
				for (PlayTile* choice : freeChoices) {
					if (choice) {
						choice->setState(3); //default recoloring
						choice = NULL;
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
	clientHandler.SendFleet(myFleet->getActiveTilePositions());
}

//Megnézi hogy a szerver el akarja-e indítani a játékot
bool GameLogic::CheckStartSignal() 
{
	return clientHandler.GetStartSignal();
}

//Bekéri a játékostól,hogy hova akar lõni,majd küldi a szervernek
PlayTile* GameLogic::Shoot(int tileindex)
{
	std::string shootPos;
	PlayTile *target;

	int offset = mySea->getTileByIndex(0).getIndexOffset();
	int enemyOffset = mySea->getEnemyIndexOffset();
	if (tileindex - enemyOffset - offset < mapSize * mapSize && tileindex - enemyOffset - offset >= 0) {
		target = &mySea->getTileByIndex(tileindex - offset);
		matchState = clientHandler.SendShot(target->getPos());
		target->setState(static_cast<int>(matchState));

		shootPos = ProcessTile(target->getPos());
		std::cout << "Your shot to " << shootPos << " was a "
			<< (matchState == ResponseState::CONTINUE_MATCH ? "miss." : (matchState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "banger!!")) << std::endl;
		return target;
	}

	return nullptr;
}

//Kapunk egy lövést az ellenféltõl
PlayTile* GameLogic::GetShoot()
{
	if (clientHandler.CheckForResponse()) {
		std::string shootPos;
		std::pair<char,int> shootCoord = clientHandler.ReceiveShot();
		matchState = clientHandler.getRecShotState();

		if (shootCoord.first == '0') {
			std::cout << "The enemy left the game!" << std::endl;
		}
		else {
			PlayTile* target = &myFleet->getTile(shootCoord);
			shootPos = ProcessTile(target->getPos());

			//Ez az if kinullázza a pozíciót,az alapján nézi meg hogy kikell-e még rajzolni egy hajót
			if (matchState != ResponseState::CONTINUE_MATCH) {
				myFleet->HitFleet(target->getPos());
			}
			target->setState(static_cast<int>(matchState));
			std::cout << "Enemy's shot to " << shootPos << " was a "
				<< (matchState == ResponseState::CONTINUE_MATCH ? "miss." : (matchState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "banger!!")) << std::endl;
			return target;
		}
	}
	return nullptr;
}

//Lövések után megnézi,hogy a szerver jelezte-e,hogy nyert valaki
int GameLogic::CheckVictoryState()
{
	if (matchState == ResponseState::WIN_PLAYER_ONE) {
		clientHandler.~ClientHandler();
		return 1;
	}
	else if (matchState == ResponseState::WIN_PLAYER_TWO) {
		clientHandler.~ClientHandler();
		return 2;
	}
	return 0;
}

//Játékosszám
int GameLogic::getPlayerNum()
{
	return playerNum;
}

//Hibamentes szöveges koordinátát konvertál át egy ideiglenes játékmezõvé
//PlayTile GameLogic::ProcessString(std::string coord)
//{
//	char coordShip[3];
//	strcpy_s(coordShip, coord.c_str());
//
//	return PlayTile(std::pair<char, int>(coordShip[0], atoi(&coordShip[1])));
//}

//Szöveges koordinátát ellenõriz,hogy jó-e
bool GameLogic::CheckString(std::string coord)
{

	if (coord.length() != 2) {
		std::cout << "Incorrect length!(must be 2)" << std::endl;
		return false;
	}

	char tmp[3];
	strcpy_s(tmp, coord.c_str());
	bool legitColumn = false;

	if (tmp[0] < ('a' + mapSize) && tmp[0] >= 'a') {
		legitColumn = true;
	}

	if (!legitColumn) {
		std::cout << "Incorrect column!(must be a-" << static_cast<char>('a' + mapSize - 1)
			<< ')' << std::endl;
		return false;
	}

	int ia = tmp[1] - '0';
	if (ia > mapSize || ia == 0) {
		std::cout << "Incorrect row!(must be 1-" << mapSize << ')' << std::endl;
		return false;
	}

	return true;
}

//Játékmezõ koordinátákból csinál szöveges formájút
std::string GameLogic::ProcessTile(const std::pair<char, int> &tile)
{
	char rowC[10];
	_itoa_s(tile.second, rowC, 10);

	std::string result;
	result.push_back(tile.first);
	result.push_back(rowC[0]);

	return result;
}

//Ellenõrzi,hogy a mezõkoordináta a pályán belül van-e
bool GameLogic::TileProcessable(const std::pair<char, int> &tile)
{
	if (tile.first > ('a' + mapSize) || tile.second > mapSize || tile.first < 'a' || tile.second <= 0) {
		return false;
	}
	return true;
}

//Játékmezõ koordinátát konvertál mezõket tartalmazó tömb indexévé
int GameLogic::ConvertCoordToTileIndex(const std::pair<char, int> &tile)
{
	int tens = (tile.first - 'a') * mapSize;
	int ones = tile.second - 1;
	return (tens + ones);
}

//Ha a játék DEBUG módba indítjuk,akkor beégetetten lerak nekünk néhány hajót
void GameLogic::PlaceShipsINDEBUG() 
{
	myFleet->PlaceShip(&myFleet->getTile(std::pair<char, int>('a', 1)), nullptr);

	//EZT JAVÍTANI KELL HOGY MENJEN
	//enemyFleet->PlaceShip(&enemyFleet->getTile(std::pair<char, int>('a', 1)), nullptr);

	myFleet->PlaceShip(&myFleet->getTile(std::pair<char, int>('c', 2)),
		&myFleet->getTile(std::pair<char, int>('d', 2)));
	myFleet->PlaceShip(&myFleet->getTile(std::pair<char, int>('c', 4)),
		&myFleet->getTile(std::pair<char, int>('c', 7)));
	myFleet->PlaceShip(&myFleet->getTile(std::pair<char, int>('e', 3)),
		&myFleet->getTile(std::pair<char, int>('e', 7)));
}

//Ha a játék DEBUG módba indítjuk,akkor beégetetten átalakít néhány játékmezõ állapotot
void GameLogic::SetTilesINDEBUG() 
{
	mySea->getTileByIndex(0).setState(1);
	mySea->getTileByIndex(1).setState(2);

	mySea->getTileByIndex(11).setState(1);
	mySea->getTileByIndex(14).setState(1);
	mySea->getTileByIndex(7).setState(1);

	mySea->getTileByIndex(0 + mySea->getEnemyIndexOffset()).setState(1);
	mySea->getTileByIndex(1 + mySea->getEnemyIndexOffset()).setState(2);
}

//Lezárja a kapcsolatot a szerverrel
void GameLogic::StopGame() {
	clientHandler.quitGame();
}