#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
#include <utility>
#include <vector>
#include <sstream>

#include "Source\ServerHandler.h"
#include "../CommonSource/TorpedoVersion.h"
#include "../CommonSource/CommonGlobals.h"

//Szerver fõosztálya
class TorpedoJatekServer
{
public:
	TorpedoJatekServer(void);
	~TorpedoJatekServer(void);

	int Start();

private:
	//Melyik szerveropció
	enum class SetupOptions {
		CLOSE_PROGRAM,
		START_SERVER,
		CHANGE_MAPSIZE,
		CHANGE_PORT,
		DUMMY_OPTION = 9999,
	};
	//Játékállapot válasz
	enum class ResponseState {
		HIT_ENEMY_SHIP=1,
		CONTINUE_MATCH,
		START_OF_GAME,
		WIN_PLAYER_ONE,
		WIN_PLAYER_TWO,
	};

	//Kliensadatok
	struct Client {
		std::stringstream name; //név
		int playerNum;	//hányadik játékos
		TCPsocket socket;	//kapcsolati socket
		std::vector<std::pair<char, int>> activeTiles;	//Mely mezõkoordinátáin vannak hajói
	}firstClient,secondClient;

	void CalcActiveTileCount();

	void Init();
	void GetShips(Client &client);
	void StartMatch();
	void UpdateSettings();
	bool CheckClientVersion(TCPsocket &connectedSocket);
	ResponseState ProcessTiles(Client &clientTiles);

	void HandleShot(Client &shooter, Client &taker);

	IPaddress ip; //ip osztálya
	Uint16 port = 27015; //portszám
	SDLNet_SocketSet socketSet = nullptr; //socket csoport
	TCPsocket server = nullptr; //szerver socket
	const int maxSockets = 3; //maximum hány socket lehet egy socketcsoportban
	int successfullyConnectedPlayers = 0; //hány kliens csatlakozott sikeresen

	std::stringstream currentSettings; //Jelenlegi beállítás szövege
	ResponseState responseState = ResponseState::START_OF_GAME; //Mi a játék állapota

	int mapSize = 7; //játékPálya mérete
	int activeTileCount = 0; //hány hajót tartalmazó mezõ van
	std::pair<char,int> targetTile; //melyik mezõkoordinátára lõtt valaki

	const TorpedoVersion serverVersion; //szerver verziószáma

};
