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

class GameLogic
{
public:
	GameLogic(void);
	~GameLogic(void);

	void Init();
	//int ActiveTileConverter(std::string tile);
	void ConnectionSetup();
	void PlaceShips();
	
	int* getActiveTiles();
protected:
	ClientSocket mySocket;

	int ProcessString(std::string coord);
	bool CheckString(std::string coord);
	bool CheckTile(int tile);
	std::string ProcessTile(int tile);
	bool TileProcessable(int tile);

	std::string output="Torpedo Jatek";
	std::string ship1PlaceText = "Where do you want to place your ship?(a1-g7)";
	std::string shipFPlaceText = "Where do you want the front of the ship to be?(a1-g7)";
	std::string shipBPlaceText = "Where do you want the back of the ship to be?\nChoices are:";

	std::string shipFront;
	std::string shipBack;

	std::string ip;
	std::string port;

	const unsigned int activeTileCount = 16;
	int activeTiles[16] = {111,163,171,113,145,146,133,143,173,174,122,123,124,135,136,137};			//beégetett!!!!

	int ship3count = 2;
	int ship2count = 3;
	int ship1count = 4;

	int choice = 0;

	char coordShip[2];
};