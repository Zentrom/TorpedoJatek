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

//Játék háttérlogikájával kapcsolatos osztály
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
	//Debug módban beégetett dolgok
	void PlaceShipsINDEBUG();
	void SetTilesINDEBUG();
	std::string ProcessTile(const std::pair<char, int> &tile);

	ClientHandler* clientHandler = new ClientHandler();	//A hálózati kapcsolat kliens-oldali vezérlõje
	std::array<int, 4> unplacedShips; //Hány hajó nincs még lerakva(külön méretekben)
	Fleet *pMyFleet; //Pointer a mi hajóseregünkre
	Fleet *pEnemyFleet; //Pointer az ellenfél hajóseregére
	Sea *pSea;	//Pointer a tengerre
	ResponseState matchState = ResponseState::START_OF_GAME; //Szerverrel való kommunikáció fázisa

	//Játékmezõ kezdetének távolsága az origótól
	const float playZoneCenterOffset = 4.0f * TorpedoGLOBAL::SeaTileScaleXZ * TorpedoGLOBAL::Scale;

	std::string output = "Torpedo Jatek"; //Kimeneti szöveg consolera
	std::string ip = "127.0.0.1"; //A szerver-ip szöveges alakja
	int port = 27015; //szerver port
	int mapSize = 7; //Tárolt pályaméret - Alapérték DEBUG módhoz
	int playerNum = 0; //Ha 1 akkor mi kezdünk,ha 2 akkor nem
	//PlaceAllyShiphez kell hogy megjegyezze,ne töröld
	PlayTile* pShipFront;
	PlayTile* pShipBack;
	bool shipFrontPlaced = false; //Hajó elejét már leraktuk-e
	std::array<PlayTile*, 4> freeChoices; //Hajó hátaknak megfelelõ pos

};