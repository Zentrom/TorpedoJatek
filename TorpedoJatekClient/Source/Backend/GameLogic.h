#pragma once

#include "ClientHandler.h"
#include "../Frontend/Fleet.h"
#include "../Frontend/Sea.h"
#include "../Globals.hpp"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <array>

//J�t�k h�tt�rlogik�j�val kapcsolatos oszt�ly
class GameLogic
{
public:
	GameLogic(Fleet& player, Fleet& enemy, Sea& sea);
	~GameLogic();

	void Init();
	void ConnectionSetup();
	void DisplayMessage(GameState gameState, int related_data);
	void DisplayError(GameState gameState, int related_data);

	bool CheckForUnplacedShips(int shipSize);
	bool CheckAnyUnplacedShipLeft();
	bool PlaceAllyShip(int tileIndex, int shipSize);
	void SendFleetToServer();

	bool CheckStartSignal();
	void QuitGame();
	int CheckVictoryState();
	PlayTile* Shoot(int tileindex);

	PlayTile* GetShoot();
	int getPlayerNum();

private:
	//Debug m�dban be�getett dolgok
	void PlaceShipsINDEBUG();
	void SetTilesINDEBUG();
	std::string ProcessTile(const std::pair<char, int> &tile);

	ClientHandler* clientHandler = new ClientHandler();	//A h�l�zati kapcsolat kliens-oldali vez�rl�je
	std::array<int, 4> unplacedShips; //H�ny haj� nincs m�g lerakva(k�l�n m�retekben)
	Fleet *pMyFleet; //Pointer a mi haj�sereg�nkre
	Fleet *pEnemyFleet; //Pointer az ellenf�l haj�sereg�re
	Sea *pSea;	//Pointer a tengerre
	ResponseState matchState = ResponseState::START_OF_GAME; //Szerverrel val� kommunik�ci� f�zisa

	//J�t�kmez� kezdet�nek t�vols�ga az orig�t�l
	const float playZoneCenterOffset = 4.0f * TorpedoGLOBAL::SeaTileScaleXZ * TorpedoGLOBAL::Scale;

	std::string output = "Torpedo Jatek"; //Kimeneti sz�veg consolera
	std::string ip = "127.0.0.1"; //A szerver-ip sz�veges alakja
	int port = 27015; //szerver port
	int mapSize = 7; //T�rolt p�lyam�ret - Alap�rt�k DEBUG m�dhoz
	int playerNum = 0; //Ha 1 akkor mi kezd�nk,ha 2 akkor nem
	//PlaceAllyShiphez kell hogy megjegyezze,ne t�r�ld
	PlayTile* pShipFront;
	PlayTile* pShipBack;
	bool shipFrontPlaced = false; //Haj� elej�t m�r leraktuk-e
	std::array<PlayTile*, 4> freeChoices; //Haj� h�taknak megfelel� pos

};