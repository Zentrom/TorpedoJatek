#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
#include <utility>
#include <array>
#include <sstream>

#include "Source\ServerHandler.h"
#include "../CommonSource/TorpedoVersion.h"

class TorpedoJatekServer
{
public:
	TorpedoJatekServer(void);
	~TorpedoJatekServer(void);

	int Start();

private:
	enum class SetupOptions {
		CLOSE_PROGRAM,
		START_SERVER,
		CHANGE_MAPSIZE,
		CHANGE_PORT,
		DUMMY_OPTION = 9999,
	};
	enum class ResponseState {
		HIT_ENEMY_SHIP=1,
		CONTINUE_MATCH,
		START_OF_GAME,
		WIN_PLAYER_ONE,
		WIN_PLAYER_TWO,
	};

	struct Client {
		std::stringstream name;
		int playerNum;
		TCPsocket socket;
		std::array<std::pair<char, int>, 16> activeTiles;
	}firstClient,secondClient;

	void Init();
	void GetShips(Client &client);
	void StartMatch();
	void UpdateSettings();
	bool CheckClientVersion(TCPsocket &connectedSocket);
	ResponseState ProcessTiles(Client &clientTiles);

	void HandleShot(Client &shooter, Client &taker);

	IPaddress ip;
	Uint16 port = 27015;
	SDLNet_SocketSet socketSet = nullptr;
	TCPsocket server = nullptr;
	const int maxSockets = 3;
	int successfullyConnectedPlayers = 0;

	std::stringstream currentSettings;
	ResponseState responseState = ResponseState::START_OF_GAME;

	int mapSize = 7;
	std::pair<char,int> targetTile;

	const TorpedoVersion serverVersion;

};
