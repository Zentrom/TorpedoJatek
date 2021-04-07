#include "GameLogic.h"

GameLogic::GameLogic(void)
{
	//if (!TorpedoGLOBAL::Debug) {
	//	for (int i = 0; i < activeTileCount; i++) {
	//		activeTiles[i] = std::pair<char,int>('0',0);
	//	}
	//}
}

GameLogic::~GameLogic(void)
{
}

int GameLogic::Init(Fleet *player,Fleet *enemy,Sea *sea)
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
		mapSize = clientHandler.getMapSize();
		//PlaceShips();
		//clientHandler.SendFleet(activeTiles.data());
	}
	return mapSize;
}


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

void GameLogic::PlaceShips()
{
	
	ship1PlaceText.push_back(static_cast<char>('a' + mapSize - 1));
	ship1PlaceText.push_back(static_cast<char>('0' + mapSize));
	ship1PlaceText.push_back(')');

	shipFPlaceText.push_back(static_cast<char>('a' + mapSize - 1));
	shipFPlaceText.push_back(static_cast<char>('0' + mapSize));
	shipFPlaceText.push_back(')');

	output = "Place your ships!/nChoose what kind of ship do you want to place down:";
	
	std::array<int,4> &unplacedShips = myFleet->getUnplacedShipCount();

	//std::pair<char, int> frontPos;
	//std::pair<char, int> backPos;
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
			<< "4. - 4tile ships left: " << unplacedShips.at(3) << std::endl;
		
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
						myFleet->PlaceShip(frontPos,backPos);
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
		else {
			std::cout << "You need to choose between 1-4!" << std::endl;
		}

	} while (std::any_of(unplacedShips.cbegin(), unplacedShips.cend(), [](int i) {return i != 0; }));
}

void GameLogic::StartMatch(std::vector<PlayTile> &myTiles, std::vector<PlayTile> &enemyTiles)
{
	playerNum= clientHandler.getPlayerNum();
	int processableTileState= 10;
	if (playerNum == 1) {
		processableTileState=Shoot();
	
		enemyTiles[ConvertCoordToTileIndex(processableTile.getPos())].setState(processableTileState);
	}
	
	while (processableTileState != 4 && processableTileState != 5) {
		processableTileState = GetShoot();
		myTiles[ConvertCoordToTileIndex(processableTile.getPos())].setState(processableTileState);
		if (processableTileState != 4 && processableTileState != 5) {
			processableTileState = Shoot();
			enemyTiles[ConvertCoordToTileIndex(processableTile.getPos())].setState(processableTileState);
		}
	}
	
	clientHandler.~ClientHandler();
	
	if ((processableTileState == 4 && playerNum==1) || (processableTileState == 5 && playerNum == 2)) {
		std::cout << "You've won the match!" << std::endl;
	}
	else if ((processableTileState == 5 && playerNum == 1)|| (processableTileState == 4 && playerNum == 2)) {
		std::cout << "You've lost the match!" << std::endl;
	}
	

}

//std::pair<char,int>* GameLogic::getActiveTiles()
//{
//	return activeTiles.data();
//}

int GameLogic::Shoot()
{
	std::string shoot;
	int newState;
	while (1) {
		std::cout << "Where do you want to shoot?(a1-" << static_cast<char>('a'+mapSize-1)
			<< mapSize <<")" << std::endl;
		std::cin >> shoot;
		if (CheckString(shoot)) {
			processableTile = ProcessString(shoot);
			newState= clientHandler.SendShot(processableTile.getPos());
			break;
		}
	}
	
	std::cout << "Your shot to " << shoot << " was a " << (newState==2? "miss." : (newState==1? "hit!" : "banger!!")) << std::endl;

	return newState;
}

int GameLogic::GetShoot()
{
	std::string shoot;
	int newState;

	processableTile = PlayTile(clientHandler.ReceiveShot());
	newState = clientHandler.getRecShotState();
	
	shoot = ProcessTile(processableTile.getPos());
	std::cout << "Enemy's shot to " << shoot << " was a " << (newState == 2 ? "miss." : (newState == 1 ? "hit!" : "banger!!")) << std::endl;

	return newState;
}

//converts errorless stringinput into tilecoords
PlayTile GameLogic::ProcessString(std::string coord)
{
	char coordShip[2];
	strcpy(coordShip,coord.c_str());

	return PlayTile(std::pair<char,int>(coordShip[0], atoi(&coordShip[1])));
}

//check errors in shipcoord string input
bool GameLogic::CheckString(std::string coord)
{

	if (coord.length() != 2) {
		std::cout << "Incorrect length!(must be 2)" << std::endl;
		return false;
	}

	char tmp[2];
	strcpy(tmp,coord.c_str());
	bool legitColumn = false;

	if(tmp[0] < ('a'+mapSize) && tmp[0] >= 'a'){
		legitColumn = true;
	}

	if (!legitColumn) {
		std::cout << "Incorrect column!(must be a-" << static_cast<char>('a'+mapSize-1)
			<< ")" << std::endl;
		return false;
	}

	int ia = tmp[1] - '0';
	if (ia > mapSize || ia==0) {
		return false;
	}

	return true;
}

//creates stringcoord out of tilecoord
std::string GameLogic::ProcessTile(std::pair<char,int> tile)
{
	char rowC[10];
	_itoa(tile.second, rowC, 10);

	std::string result;
	result.push_back(tile.first);
	result.push_back(rowC[0]);

	return result;
}

//checks if tilecoord is legal
bool GameLogic::TileProcessable(std::pair<char,int> tile)
{
	if(tile.first>('a'+mapSize) || tile.second>mapSize || tile.first<'a' || tile.second <= 0){
		return false;
	}
	return true;
}

int GameLogic::ConvertCoordToTileIndex(std::pair<char,int> tile)
{
	int tens= (tile.first-'a') * mapSize;
	int ones= tile.second-1;
	return (tens+ones);
}

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