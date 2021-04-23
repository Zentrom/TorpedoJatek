#include "GameLogic.h"

GameLogic::GameLogic(void)
{
}

GameLogic::~GameLogic(void)
{
}

//Inicializálja a háttérlogikát
int GameLogic::Init(Fleet *player, Fleet *enemy, Sea *sea)
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
	if (!TorpedoGLOBAL::Debug) {
		PlaceShips();
		clientHandler.SendFleet(myFleet->getActiveTilePositions());
	}
	else {
		PlaceShipsINDEBUG();
	}
}

//Lerakja a mi hajóinkat a pályára
void GameLogic::PlaceShips()
{
	ship1PlaceText.push_back(static_cast<char>('a' + mapSize - 1));
	ship1PlaceText.push_back(static_cast<char>('0' + mapSize));
	ship1PlaceText.push_back(')');

	shipFPlaceText.push_back(static_cast<char>('a' + mapSize - 1));
	shipFPlaceText.push_back(static_cast<char>('0' + mapSize));
	shipFPlaceText.push_back(')');

	output = "Place your ships!/nChoose what kind of ship do you want to place down:";

	std::array<int, 4> &unplacedShips = myFleet->getUnplacedShipCount();

	PlayTile tmpFront;
	PlayTile tmpBack;

	PlayTile* frontPos;
	PlayTile* backPos;
	do {

		backPos = nullptr;

		std::cout << '\n' <<
			output << '\n'
			<< "1. - 1tile ships left: " << unplacedShips.at(0) << '\n'
			<< "2. - 2tile ships left: " << unplacedShips.at(1) << '\n'
			<< "3. - 3tile ships left: " << unplacedShips.at(2) << '\n'
			<< "4. - 4tile ships left: " << unplacedShips.at(3) << '\n'
			<< "0. - Quit game!" << std::endl;

		std::cin >> choice;

		if (choice > 0 && choice <= 4) {
			if (unplacedShips.at(choice - 1) > 0) {
				std::cout << (choice == 1 ? ship1PlaceText : shipFPlaceText) << std::endl;
				std::cin >> shipFront;

				if (CheckString(shipFront)) {
					tmpFront = ProcessString(shipFront);
					if (myFleet->CheckTile(tmpFront)) {
						frontPos = &myFleet->getTile(tmpFront.getPos());
						if (choice > 1) {
							std::array<PlayTile*, 4> freeChoices = myFleet->getFreeBacks(*frontPos, choice - 1);
							if (std::none_of(freeChoices.cbegin(), freeChoices.cend(), [](PlayTile* ptr) {return ptr; }))
							{
								std::cout << "No position available for the back of the ship!" << std::endl;
								continue;
							}
							else {
								bool foundInputInChoices = false;
								while (!foundInputInChoices) {
									std::cout << shipBPlaceText;
									for (PlayTile* choisz : freeChoices) {
										if (choisz) {
											std::cout << ' ' << choisz->getPos().first << choisz->getPos().second;
										}
									}
									std::cout << std::endl;
									std::cin >> shipBack;

									if (CheckString(shipBack)) {
										tmpBack = ProcessString(shipBack);
										backPos = &myFleet->getTile(tmpBack.getPos());
										for (PlayTile* choisz : freeChoices) {
											if (choisz && choisz == backPos) {
												foundInputInChoices = true;
											}
										}
									}
								}
							}
						}
						myFleet->PlaceShip(frontPos, backPos);
						--unplacedShips.at(choice - 1);
					}
					else {
						std::cout << "Tile is not empty!" << std::endl;
					}
				}
			}
			else {
				std::cout << "You can't place down any more ships of " << choice << " size!" << std::endl;
			}
		}
		else if (choice == 0 && !std::cin.fail()) {
			clientHandler.quitGame();
		}
		else {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "You need to choose between 0-4!" << std::endl;
		}

	} while (std::any_of(unplacedShips.cbegin(), unplacedShips.cend(), [](int i) {return i != 0; }));
}

//Elkezdi a játékmenetet két játékos között
void GameLogic::StartMatch(std::vector<PlayTile> &myTiles, std::vector<PlayTile> &enemyTiles)
{
	std::cout << "Waiting for server to start the match." << std::endl;
	clientHandler.GetStartSignal();

	ResponseState processableTileState = ResponseState::START_OF_GAME; //statenel 1-piros 2-sarga 3-kek 4-nyert 5-vesztett
	if (playerNum == 1) {
		processableTileState = Shoot();
		enemyTiles[ConvertCoordToTileIndex(processableTile.getPos())].setState(static_cast<int>(processableTileState));
	}

	while (processableTileState != ResponseState::WIN_PLAYER_ONE && processableTileState != ResponseState::WIN_PLAYER_TWO) {
		processableTileState = GetShoot();
		myTiles[ConvertCoordToTileIndex(processableTile.getPos())].setState(static_cast<int>(processableTileState));
		if (processableTileState != ResponseState::WIN_PLAYER_ONE && processableTileState != ResponseState::WIN_PLAYER_TWO) {
			processableTileState = Shoot();
			enemyTiles[ConvertCoordToTileIndex(processableTile.getPos())].setState(static_cast<int>(processableTileState));
		}
	}

	clientHandler.~ClientHandler();

	if ((processableTileState == ResponseState::WIN_PLAYER_ONE && playerNum == 1) || (processableTileState == ResponseState::WIN_PLAYER_TWO && playerNum == 2)) {
		std::cout << "You've won the match!" << std::endl;
	}
	else if ((processableTileState == ResponseState::WIN_PLAYER_TWO && playerNum == 1) || (processableTileState == ResponseState::WIN_PLAYER_ONE && playerNum == 2)) {
		std::cout << "You've lost the match!" << std::endl;
	}

}

//Bekéri a játékostól,hogy hova akar lõni,majd küldi a szervernek
ResponseState GameLogic::Shoot()
{
	std::string shoot;
	ResponseState newState;
	while (1) {
		std::cout << "Where do you want to shoot?(a1-" << static_cast<char>('a' + mapSize - 1)
			<< mapSize << ")\n(Or enter 0 to quit!)" << std::endl;
		std::cin >> shoot;
		if (shoot == "0") {
			clientHandler.quitGame();
		}
		if (CheckString(shoot)) {
			processableTile = ProcessString(shoot);
			newState = clientHandler.SendShot(processableTile.getPos());
			break;
		}
	}

	std::cout << "Your shot to " << shoot << " was a "
		<< (newState == ResponseState::CONTINUE_MATCH ? "miss." : (newState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "banger!!")) << std::endl;

	return newState;
}

//Kapunk egy lövést az ellenféltõl
ResponseState GameLogic::GetShoot()
{
	std::string shoot;
	ResponseState newState;

	processableTile = PlayTile(clientHandler.ReceiveShot());
	newState = clientHandler.getRecShotState();

	if (newState != ResponseState::CONTINUE_MATCH) {
		myFleet->HitFleet(processableTile.getPos());
	}

	if (processableTile.getPos().first == '0') {
		std::cout << "The enemy left the game!" << std::endl;
	}
	else {
		shoot = ProcessTile(processableTile.getPos());
		std::cout << "Enemy's shot to " << shoot << " was a "
			<< (newState == ResponseState::CONTINUE_MATCH ? "miss." : (newState == ResponseState::HIT_ENEMY_SHIP ? "hit!" : "banger!!")) << std::endl;
	}
	return newState;
}

//Hibamentes szöveges koordinátát konvertál át egy ideiglenes játékmezõvé
PlayTile GameLogic::ProcessString(std::string coord)
{
	char coordShip[3];
	strcpy_s(coordShip, coord.c_str());

	return PlayTile(std::pair<char, int>(coordShip[0], atoi(&coordShip[1])));
}

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
void GameLogic::PlaceShipsINDEBUG() {
	myFleet->PlaceShip(&myFleet->getTile(std::pair<char, int>('a', 1)), nullptr);

	enemyFleet->PlaceShip(&enemyFleet->getTile(std::pair<char, int>('a', 1)), nullptr);

	myFleet->PlaceShip(&myFleet->getTile(std::pair<char, int>('c', 2)),
		&myFleet->getTile(std::pair<char, int>('d', 2)));
	myFleet->PlaceShip(&myFleet->getTile(std::pair<char, int>('c', 4)),
		&myFleet->getTile(std::pair<char, int>('c', 7)));
	myFleet->PlaceShip(&myFleet->getTile(std::pair<char, int>('e', 3)),
		&myFleet->getTile(std::pair<char, int>('e', 7)));
}