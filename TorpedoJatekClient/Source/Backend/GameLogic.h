#pragma once

#include "ClientHandler.h"
#include "../Frontend/Fleet.h"
#include "../Frontend/Sea.h"
#include "../Globals.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <array>
#include <algorithm>

//J�t�k h�tt�rlogik�j�val kapcsolatos oszt�ly
class GameLogic
{
public:
	GameLogic(void);
	~GameLogic(void);

	int Init(Fleet *player, Fleet *enemy, Sea *sea);
	void ConnectionSetup();
	void InitGame();
	void PlaceShips();
	void StartMatch(std::vector<PlayTile> &myTiles, std::vector<PlayTile> &enemyTiles);

	ResponseState Shoot();
	ResponseState GetShoot();
protected:
	ClientHandler clientHandler;	//A h�l�zati kapcsolat kliens-oldali vez�rl�je

	void PlaceShipsINDEBUG();

	bool CheckString(std::string coord);
	PlayTile ProcessString(std::string coord);
	std::string ProcessTile(const std::pair<char, int> &tile);
	bool TileProcessable(const std::pair<char, int> &tile);
	int ConvertCoordToTileIndex(const std::pair<char, int> &tile);

	std::string output = "Torpedo Jatek";
	std::string ship1PlaceText = "Where do you want to place your ship?(a1-";
	std::string shipFPlaceText = "Where do you want the front of the ship to be?(a1-";
	std::string shipBPlaceText = "Where do you want the back of the ship to be?\nChoices are:";

	std::string shipFront;	//Haj� v�geit t�rol� inputadat
	std::string shipBack;

	std::string ip = "127.0.0.1";	//A szerver-ip sz�veges alakja
	int port = 27015; //szerver port

	Fleet *myFleet;	//Pointer a mi haj�sereg�nkre
	Fleet *enemyFleet;	//Pointer az ellenf�l haj�sereg�re
	Sea *mySea;	//Pointer a tengerre

	int mapSize = 7;	//T�rolt p�lyam�ret
	int choice = 0; //Felhaszn�l�i input,mikor d�nteni kell

	int playerNum;	//Ha 1 akkor mi kezd�nk,ha 2 akkor nem

	PlayTile processableTile; //Feldolgoz�sra v�r� j�t�kmez�
};