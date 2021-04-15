#pragma once

#include <iostream>
#include <utility>

#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
#include <vector>

#include "../Globals.h"
#include "../../../CommonSource/ConnectionHandler.h"
#include "../../../CommonSource/TorpedoVersion.h"
#include "../../../CommonSource/CommonGlobals.h"

//A kliens oldali kapcsolattartó osztály
class ClientHandler : public ConnectionHandler
{
public:
	ClientHandler(void);
	~ClientHandler(void);

	bool Init(std::string ipString,int portNr);
	void SendFleet(std::vector<std::pair<char,int>> activeTilePositions);
	int getMapSize();
	int getPlayerNum();
	void getStartSignal();
	ResponseState SendShot(std::pair<char,int> tile);
	std::pair<char,int> ReceiveShot();
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
