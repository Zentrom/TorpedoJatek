#pragma once

//#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <iostream>
#include <sstream>
#include <string>

#include "../../Utils/gVertexBuffer.h"
#include "../../Utils/gShaderProgram.h"
#include "../../Utils/gCamera.h"

#include "ClientSocket.h"
#include "../Frontend/PlayTile.h"

#include "../Globals.h"

class GameLogic
{
public:
	GameLogic(void);
	~GameLogic(void);

	void Init();
	//int ActiveTileConverter(std::string tile);
	void ConnectionSetup();
	void PlaceShips();
	void StartMatch(PlayTile *myTiles,PlayTile *enemyTiles);

	int* getActiveTiles();

	int Shoot();
	int GetShoot();
protected:
	ClientSocket mySocket;

	int ProcessString(std::string coord);
	bool CheckString(std::string coord);
	bool CheckTile(int tile);
	std::string ProcessTile(int tile);
	bool TileProcessable(int tile);
	int ConvertCoordToTileIndex(int tile);

	std::string output="Torpedo Jatek";
	std::string ship1PlaceText = "Where do you want to place your ship?(a1-G7)";
	std::string shipFPlaceText = "Where do you want the front of the ship to be?(a1-G7)";
	std::string shipBPlaceText = "Where do you want the back of the ship to be?\nChoices are:";

	std::string shipFront;
	std::string shipBack;

	std::string ip="127.0.0.1";
	int port=27015;

	char theColumns[32] = { 'a','b','d','e','f','g' ,'h' ,'i' ,'j' ,'k' ,'l' ,'m' ,'n' ,'o' ,'p' ,'q'
		,'r' ,'s' ,'t' ,'u' ,'v' ,'w' ,'x' ,'y' ,'z' };

	const unsigned int activeTileCount = 16;
	//int activeTiles[16] = {111,163,171,113,145,146,133,143,173,174,122,123,124,135,136,137};			//beégetett!!!!
	int activeTiles[16] = { 10101,10603,10701,10103,10405,10406,10303,10403,10703,10704,10202,10203,10204,10305,10306,10307 };

	int ship3count = 2;
	int ship2count = 3;
	int ship1count = 4;

	int choice = 0;

	char coordShip[3];

	int playerNum;
	int receivedShoot=99999; //1-win 2-loose

	int processableTile; //statenel 1-piros 2-sarga 3-kek 4-nyert 5-vesztett
};