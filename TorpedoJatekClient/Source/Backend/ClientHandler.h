#pragma once

#include "../Globals.h"
#include "../../../CommonSource/ConnectionHandler.h"
#include "../../../CommonSource/TorpedoVersion.h"
#include "../../../CommonSource/CommonGlobals.h"

#include <SDL.h>
#include <SDL_net.h>

#include <iostream>
#include <utility>
#include <cstring>
#include <vector>

//A kliens oldali kapcsolattart� oszt�ly
class ClientHandler : public ConnectionHandler
{
public:
	ClientHandler(void);
	~ClientHandler(void);

	bool Init(std::string ipString = "127.0.0.1", int portNr = 27015);
	void SendFleet(const std::vector<std::pair<char, int>> &activeTilePositions);
	int GetMapSize();
	int GetPlayerNum();
	void GetStartSignal();
	ResponseState SendShot(const std::pair<char, int> &tile);
	std::pair<char, int> ReceiveShot();
	void quitGame();

	ResponseState getRecShotState();

protected:
	IPaddress ip;	//ip-c�m
	TCPsocket mySocket;	//kliens oldali socket

	int receivedBytes = 0;	//szervert�l �rkezett byte mennyis�ge
	int sentBytes = 0; //szervernek �tk�ld�tt byte mennyis�ge
	ResponseState stateResult = ResponseState::START_OF_GAME; //Mi a j�t�k �llapota
	MessageType sentMessageType = MessageType::ESTIMATED; //Szervert�l v�rt adatot k�ld�nk-e

	const TorpedoVersion clientVersion; //a kliens verzi�sz�ma
};
