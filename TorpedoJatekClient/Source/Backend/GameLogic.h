#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <array>
#include <algorithm>

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include "ClientHandler.h"
//#include "../Frontend/PlayTile.h"
#include "../Frontend/Fleet.h"
#include "../Frontend/Sea.h"
#include "../Globals.h"

class GameLogic
{
public:
	GameLogic(void);
	~GameLogic(void);

	int Init(Fleet *player,Fleet *enemy,Sea *sea);
	void ConnectionSetup();
	void InitGame();
	void PlaceShips();
	void StartMatch(std::vector<PlayTile> &myTiles,std::vector<PlayTile> &enemyTiles);

	//std::pair<char,int>* getActiveTiles();

	int Shoot();
	int GetShoot();
protected:
	ClientHandler clientHandler;
	
	void PlaceShipsINDEBUG();

	bool CheckString(std::string coord);
	PlayTile ProcessString(std::string coord);
	std::string ProcessTile(std::pair<char,int> tile);
	bool TileProcessable(std::pair<char,int> tile);
	int ConvertCoordToTileIndex(std::pair<char,int> tile);

	std::string output="Torpedo Jatek";
	std::string ship1PlaceText = "Where do you want to place your ship?(a1-";
	std::string shipFPlaceText = "Where do you want the front of the ship to be?(a1-";
	std::string shipBPlaceText = "Where do you want the back of the ship to be?\nChoices are:";

	std::string shipFront;
	std::string shipBack;

	std::string ip="127.0.0.1";
	int port=27015;

	Fleet *myFleet;
	Fleet *enemyFleet;
	Sea *mySea;
	//const int activeTileCount = 16;
	//beégetett!!!!
	//std::array<std::pair<char, int>, 16> activeTiles = { std::pair<char,int>('a',1),std::pair<char,int>('f',3),
	//	std::pair<char,int>('g',1), std::pair<char,int>('a',3), std::pair<char,int>('d',5), 
	//	std::pair<char,int>('d',6), std::pair<char,int>('c',3), std::pair<char,int>('d',3), 
	//	std::pair<char,int>('g',3), std::pair<char,int>('g',4), std::pair<char,int>('b',2), 
	//	std::pair<char,int>('b',3), std::pair<char,int>('b',4), std::pair<char,int>('c',5), 
	//	std::pair<char,int>('c',6), std::pair<char,int>('c',7), };

	//int ship3count = 2;
	//int ship2count = 3;
	//int ship1count = 4;

	int mapSize = 7;
	int choice = 0;

	int playerNum;
	int receivedShoot=99999; //1-win 2-loose

	PlayTile processableTile; //statenel 1-piros 2-sarga 3-kek 4-nyert 5-vesztett
};