#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
#include <utility>
#include <array>
#include <sstream>

class TorpedoJatekServer
{
public:
	TorpedoJatekServer(void);
	~TorpedoJatekServer(void);

	int Start();

protected:
	enum class SetupOptions {
		CLOSE_PROGRAM,
		START_SERVER,
		CHANGE_MAPSIZE,
		DUMMY_OPTION = 9999,
	};
	enum class ResponseState {
		HIT_ENEMY_SHIP=1,
		CONTINUE_MATCH,
		START_OF_GAME,
		WIN_PLAYER_ONE,
		WIN_PLAYER_TWO,
	};
	void Init();
	void StartMatch();
	void UpdateSettings();
	void ReportErrorAndExit(char functionName[],int exitCode);
	bool CheckClientVersion(TCPsocket &connectedSocket);
	ResponseState ProcessTiles(int playerNum);

	IPaddress ip;
	Uint16 port = 27015;
	SDLNet_SocketSet socketSet = nullptr;
	TCPsocket server = nullptr;
	IPaddress *firstClientAddress;
	TCPsocket firstClient = nullptr;
	IPaddress *secondClientAddress;
	TCPsocket secondClient = nullptr;
	const int maxSockets = 3;

	std::stringstream currentSettings;
	ResponseState responseState = ResponseState::START_OF_GAME;
	bool closeConnection = false;
	int sentBytes;
	int receivedBytes;

	int mapSize = 7;
	//int activeTilesOne[16];
	//int activeTilesTwo[16];
	std::array<std::pair<char, int>, 16> activeTilesPlayerOne;
	std::array<std::pair<char, int>, 16> activeTilesPlayerTwo;
	std::pair<char,int> targetTile;
	//int responseState=3;
	
	//serververzio
	struct TorpedoVersion {
		unsigned int majorVersion = 0;
		unsigned int betaVersion = 0;
		unsigned int alphaVersion = 1;
		char experimentalVersion = 'd';
	}const serverVersion;

};
