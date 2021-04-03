#include "GameLogic.h"

GameLogic::GameLogic(void)
{
	if (!TorpedoGLOBAL::Debug) {
		for (int i = 0; i < activeTileCount; i++) {
			activeTiles[i] = std::pair<char,int>('0',0);
		}
	}
}

GameLogic::~GameLogic(void)
{
}

void GameLogic::Init()
{
	std::cout << "-----------------------------------------------" << std::endl
		<< output << std::endl
		<< "-----------------------------------------------" << std::endl
		<< std::endl;

	if (!TorpedoGLOBAL::Debug) {
		ConnectionSetup();
		PlaceShips();
		mySocket.SendFleet(activeTiles.data());

	}
}


void GameLogic::ConnectionSetup()
{
	
	output = "Establishing connection.";
	std::cout << output << std::endl;

	output = "Server ip:";
	std::cout << output << std::endl;
	std::cin >> ip;

	output = "Server port:";
	std::cout << output << std::endl;
	std::cin >> port;
	
	mySocket.Init(ip,port);
}

void GameLogic::PlaceShips()
{
	output = "Place your ships!/nChoose what kind of ship do you want to place down:";
	
	do {
		std::cout << std::endl <<
			output << std::endl
			<< "1. - 3tile ships left: " << ship3count << std::endl
			<< "2. - 2tile ships left: " << ship2count << std::endl
			<< "3. - 1tile ships left: " << ship1count << std::endl;

		std::cin >> choice;

		std::pair<char,int> tmpTile;
		std::string tmpBack[4] = {" "," "," "," "};
		//int tmpMiddle[4] = { 0,0,0,0 };
		std::pair<char, int> tmpMiddle[4] = {std::pair<char,int>('0',0),std::pair<char,int>('0',0),
			std::pair<char,int>('0',0), std::pair<char,int>('0',0) };
		switch (choice) {
		case 1:
			if (ship3count != 0) {
				std::cout << shipFPlaceText << std::endl;
				std::cin >> shipFront;

				if (CheckString(shipFront)) {
					tmpTile = ProcessString(shipFront);

					if (CheckTile(tmpTile)) {

						if (TileProcessable(std::pair<char,int>(tmpTile.first+1,tmpTile.second)) 
							&& TileProcessable(std::pair<char, int>(tmpTile.first + 2, tmpTile.second))) {
							if (CheckTile(std::pair<char, int>(tmpTile.first + 1, tmpTile.second))
								&& CheckTile(std::pair<char, int>(tmpTile.first + 2, tmpTile.second)))
								tmpBack[0] = ProcessTile(std::pair<char, int>(tmpTile.first + 2, tmpTile.second));
								tmpMiddle[0] = std::pair<char, int>(tmpTile.first + 1, tmpTile.second);
						}
						if (TileProcessable(std::pair<char, int>(tmpTile.first - 1, tmpTile.second))
							&& TileProcessable(std::pair<char, int>(tmpTile.first - 2, tmpTile.second))) {
							if (CheckTile(std::pair<char, int>(tmpTile.first - 1, tmpTile.second))
								&& CheckTile(std::pair<char, int>(tmpTile.first - 2, tmpTile.second)))
								tmpBack[1] = ProcessTile(std::pair<char, int>(tmpTile.first - 2, tmpTile.second));
								tmpMiddle[1] = std::pair<char, int>(tmpTile.first - 1, tmpTile.second);
						}
						if (TileProcessable(std::pair<char, int>(tmpTile.first, tmpTile.second+1))
							&& TileProcessable(std::pair<char, int>(tmpTile.first, tmpTile.second + 2))) {
							if (CheckTile(std::pair<char, int>(tmpTile.first, tmpTile.second + 1))
								&& CheckTile(std::pair<char, int>(tmpTile.first, tmpTile.second + 2)))
								tmpBack[2] = ProcessTile(std::pair<char, int>(tmpTile.first, tmpTile.second + 2));
								tmpMiddle[2] = std::pair<char, int>(tmpTile.first, tmpTile.second + 1);
						}
						if (TileProcessable(std::pair<char, int>(tmpTile.first, tmpTile.second - 1))
							&& TileProcessable(std::pair<char, int>(tmpTile.first, tmpTile.second - 2))) {
							if (CheckTile(std::pair<char, int>(tmpTile.first, tmpTile.second - 1))
								&& CheckTile(std::pair<char, int>(tmpTile.first, tmpTile.second - 2)))
								tmpBack[3] = ProcessTile(std::pair<char, int>(tmpTile.first, tmpTile.second - 2));
								tmpMiddle[3] = std::pair<char, int>(tmpTile.first, tmpTile.second - 1);
						}

						bool arrayHasElem = false;
						for (int i = 0; i < 4; i++) {
							if (tmpBack[i] != " " && tmpMiddle[i].first!='0') { arrayHasElem = true; break; }
						}
						if (arrayHasElem) {
							bool tmpFound = false;
							int midIndex = 0;
							do {
								std::cout << shipBPlaceText << tmpBack[0] << tmpBack[1] << tmpBack[2] << tmpBack[3] << std::endl;
								std::cin >> shipBack;


								for (int i = 0; i < 4; i++) {
									if (shipBack == tmpBack[i]) {
										tmpFound = true;
										midIndex = i;
										break;
									}
								}
							} while (!tmpFound);

							for (int i = 0; i < activeTileCount; i++) {
								if (activeTiles[i].first == '0') {
									activeTiles[i] = tmpTile;
									break;
								}
							}
							tmpTile = ProcessString(shipBack);
							for (int i = 0; i < activeTileCount; i++) {
								if (activeTiles[i].first == '0') {
									activeTiles[i] = tmpTile;
									break;
								}
							}
							for (int i = 0; i < activeTileCount; i++) {
								if (activeTiles[i].first == '0') {
									activeTiles[i] = tmpMiddle[midIndex];
									break;
								}
							}
							ship3count--;
							std::cout << "Ship placed!" << std::endl;
						}
						else {
							std::cout << "There aren't any free tiles for the back(middle) of this ship!" << std::endl;
						}
						for (int i = 0; i < 4; i++) {
							tmpBack[i] = " ";
							tmpMiddle[i] = std::pair<char,int>('0',0);
						}
					}
					else {
						std::cout << "Tile is not empty!" << std::endl;
					}
				}
			}
			break;
		case 2:
			if (ship2count != 0) {
				std::cout << shipFPlaceText << std::endl;
				std::cin >> shipFront;

				if (CheckString(shipFront)) {
					tmpTile = ProcessString(shipFront);

					if (CheckTile(tmpTile)) {

						if (TileProcessable(std::pair<char, int>(tmpTile.first + 1, tmpTile.second))) {
							if (CheckTile(std::pair<char, int>(tmpTile.first + 1, tmpTile.second)))
								tmpBack[0] = ProcessTile(std::pair<char, int>(tmpTile.first + 1, tmpTile.second));
						}
						if (TileProcessable(std::pair<char, int>(tmpTile.first - 1, tmpTile.second))) {
							if (CheckTile(std::pair<char, int>(tmpTile.first - 1, tmpTile.second)))
								tmpBack[1] = ProcessTile(std::pair<char, int>(tmpTile.first - 1, tmpTile.second));
						}
						if (TileProcessable(std::pair<char, int>(tmpTile.first, tmpTile.second+1))) {
							if (CheckTile(std::pair<char, int>(tmpTile.first, tmpTile.second + 1)))
								tmpBack[2] = ProcessTile(std::pair<char, int>(tmpTile.first, tmpTile.second + 1));
						}
						if (TileProcessable(std::pair<char, int>(tmpTile.first, tmpTile.second - 1))) {
							if (CheckTile(std::pair<char, int>(tmpTile.first, tmpTile.second - 1)))
								tmpBack[3] = ProcessTile(std::pair<char, int>(tmpTile.first, tmpTile.second - 1));
						}
						bool arrayHasElem = false;
						for (int i = 0; i < 4; i++) {
							if (tmpBack[i] != " ") { arrayHasElem = true; break; }
						}
						if (arrayHasElem) {
							bool tmpFound = false;
							do {
								std::cout << shipBPlaceText << tmpBack[0] << " " << tmpBack[1] << " " << tmpBack[2] << " " << tmpBack[3] << std::endl;
								std::cin >> shipBack;


								for (int i = 0; i < 4; i++) {
									if (shipBack == tmpBack[i]) {
										tmpFound = true;
										break;
									}
								}
							} while (!tmpFound);

							for (int i = 0; i < activeTileCount; i++) {
								if (activeTiles[i].first == '0') {
									activeTiles[i] = tmpTile;
									break;
								}
							}
							tmpTile = ProcessString(shipBack);
							for (int i = 0; i < activeTileCount; i++) {
								if (activeTiles[i].first == '0') {
									activeTiles[i] = tmpTile;
									break;
								}
							}
							ship2count--;
							std::cout << "Ship placed!" << std::endl;
						}
						else {
							std::cout << "There aren't any free tiles for the back of this ship!" << std::endl;
						}
						tmpBack[0] = " ";
						tmpBack[1] = " ";
						tmpBack[2] = " ";
						tmpBack[3] = " ";
					}
					else {
						std::cout << "Tile is not empty!" << std::endl;
					}
				}
			}
			break;
		case 3:
			if (ship1count != 0) {
				std::cout << ship1PlaceText << std::endl;
				std::cin >> shipFront;

				shipBack = shipFront;

				if (CheckString(shipFront)) {
					tmpTile = ProcessString(shipFront);
					if (CheckTile(tmpTile)) {
						for (int i = 0; i < activeTileCount; i++) {
							if (activeTiles[i].first == '0') {
								activeTiles[i] = tmpTile;
								break;
							}
						}
						ship1count--;
						std::cout << "Ship placed!" << std::endl;
					}
					else {
						std::cout << "Tile is not empty!" << std::endl;
					}
				}
			}
			break;
		default:
			std::cout << "You need to choose between 1-3!" << std::endl;
			break;
		}

		

	} while (ship3count!=0 ||ship2count!=0 || ship1count!=0);
}

void GameLogic::StartMatch(PlayTile *myTiles, PlayTile *enemyTiles)
{
	playerNum=mySocket.getPlayerNum();
	int processableTileState= 10;
	if (playerNum == 1) {
		processableTileState=Shoot();

		enemyTiles[ConvertCoordToTileIndex(processableTile)].setState(processableTileState);
	}

	while (processableTileState != 4 && processableTileState != 5) {
		processableTileState = GetShoot();
		myTiles[ConvertCoordToTileIndex(processableTile)].setState(processableTileState);
		if (processableTileState != 4 && processableTileState != 5) {
			processableTileState = Shoot();
			enemyTiles[ConvertCoordToTileIndex(processableTile)].setState(processableTileState);
		}
	}

	mySocket.~ClientHandler();

	if ((processableTileState == 4 && playerNum==1) || (processableTileState == 5 && playerNum == 2)) {
		std::cout << "You've won the match!" << std::endl;
	}
	else if ((processableTileState == 5 && playerNum == 1)|| (processableTileState == 4 && playerNum == 2)) {
		std::cout << "You've lost the match!" << std::endl;
	}
	

}

std::pair<char,int>* GameLogic::getActiveTiles()
{
	return activeTiles.data();
}

int GameLogic::Shoot()
{
	std::string shoot;
	//int sentData;
	int newState;
	while (1) {
		std::cout << "Where do you want to shoot?(a1-" << static_cast<char>('a'+TorpedoGLOBAL::MapSize-1)//theColumns[TorpedoGLOBAL::MapSize -1] 
			<< TorpedoGLOBAL::MapSize <<")" << std::endl;
		std::cin >> shoot;
		if (CheckString(shoot)) {
			processableTile = ProcessString(shoot);
			newState=mySocket.SendShot(processableTile);
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
	//int receivedShotTile;

	processableTile = mySocket.ReceiveShot();
	newState = mySocket.getRecShotState();

	shoot = ProcessTile(processableTile);
	std::cout << "Enemy's shot to " << shoot << " was a " << (newState == 2 ? "miss." : (newState == 1 ? "hit!" : "banger!!")) << std::endl;

	return newState;
}

//converts errorless stringinput into tilecoords
std::pair<char,int> GameLogic::ProcessString(std::string coord)
{
	char coordShip[2];
	strcpy(coordShip,coord.c_str());

	//int first;
	//for (int i = 0; i < TorpedoGLOBAL::MapSize; i++) {
	//	if (coordShip[0] == theColumns[i]) {
	//		first = i + 1;
	//	}
	//}

	//int second;
	//if (TorpedoGLOBAL::MapSize < 10) {
	//	second = atoi(&coordShip[1]);
	//}
	//else {
	//	second = atoi(&coordShip[1]) * 10 +atoi(&coordShip[2]);
	//}

	//int result = 10000 + first * 100 + second;

	return std::pair<char,int>(coordShip[0], atoi(&coordShip[1]));
}

//check errors in shipcoord string input
bool GameLogic::CheckString(std::string coord)
{
	//if (TorpedoGLOBAL::MapSize < 10) {
	if (coord.length() != 2) {
		std::cout << "Incorrect length!(must be 2)" << std::endl;
		return false;
	}
	//}
	//else {
	//	if (coord.length() > 3 || coord.length() < 2) {
	//		std::cout << "Incorrect length!(must be 2 or 3)" << std::endl;
	//		return false;
	//	}
	//}

	char tmp[2];
	strcpy(tmp,coord.c_str());
	bool legitColumn = false;
	//for (int i = 0; i < TorpedoGLOBAL::MapSize; i++) {
		//if (tmp[0] == theColumns[i]) {
	if(tmp[0] < ('a'+TorpedoGLOBAL::MapSize) || tmp[0] >= 'a'){
		legitColumn = true;
	}
	//}
	if (!legitColumn) {
		std::cout << "Incorrect column!(must be a-" << static_cast<char>('a'+TorpedoGLOBAL::MapSize-1)//theColumns[TorpedoGLOBAL::MapSize - 1] 
			<< ")" << std::endl;
		return false;
	}

	//if (TorpedoGLOBAL::MapSize < 10) {
	int ia = tmp[1] - '0';
	if (ia > TorpedoGLOBAL::MapSize) {
		return false;
	}
	//}
	//else {
	//	int ia = (tmp[1] - '0') * 10 + (tmp[2] - '0');
	//	if (ia > TorpedoGLOBAL::MapSize) {
	//		return false;
	//	}
	//}

	return true;
}

//check if the tile is free
bool GameLogic::CheckTile(std::pair<char,int> tile)
{
	for (int i = 0; i < activeTileCount; i++) {
		if (activeTiles[i] == tile) {
			return false;
		}
	}
	return true;
}

//creates stringcoord out of tilecoord
std::string GameLogic::ProcessTile(std::pair<char,int> tile)
{
	//int column = (tile % 10000) / 100;
	//int row = tile % 100;
	//char col = theColumns[column-1];

	char rowC[10];
	//_itoa(row, rowC,10);
	_itoa(tile.second, rowC, 10);

	std::string result;
	//result.push_back(col);
	result.push_back(tile.first);
	result.push_back(rowC[0]);

	return result;
}

//checks if tilecoord is legal
bool GameLogic::TileProcessable(std::pair<char,int> tile)
{
	//if((tile%10000)/100>TorpedoGLOBAL::MapSize || tile%100>TorpedoGLOBAL::MapSize || (tile%10000)/100 == 0 || tile%100 == 0){
	if(tile.first>('a'+TorpedoGLOBAL::MapSize) || tile.second>TorpedoGLOBAL::MapSize || tile.first<'a' || tile.second <= 0){
		return false;
	}
	return true;
}

int GameLogic::ConvertCoordToTileIndex(std::pair<char,int> tile)
{
	int tens= (tile.first-'a') * TorpedoGLOBAL::MapSize;
	int ones= tile.second-1;
	return (tens+ones);
}
