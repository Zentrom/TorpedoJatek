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

//A kliens oldali kapcsolattartó osztály
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
	IPaddress ip;	//ip-cím
	TCPsocket mySocket;	//kliens oldali socket

	int receivedBytes = 0;	//szervertõl érkezett byte mennyisége
	int sentBytes = 0; //szervernek átküldött byte mennyisége
	ResponseState stateResult = ResponseState::START_OF_GAME; //Mi a játék állapota
	MessageType sentMessageType = MessageType::ESTIMATED; //Szervertõl várt adatot küldünk-e

	const TorpedoVersion clientVersion; //a kliens verziószáma
};
