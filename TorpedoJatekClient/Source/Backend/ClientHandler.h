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

class ClientHandler : public ConnectionHandler
{
public:
	ClientHandler(void);
	~ClientHandler(void);

	bool Init(std::string ipString,int portNr);
	void SendFleet(std::vector<std::pair<char,int>> activeTilePositions);
	int getMapSize();
	int getPlayerNum();
	int SendShot(std::pair<char,int> tile);
	std::pair<char,int> ReceiveShot();

	int getRecShotState();

protected:
	IPaddress ip;
	TCPsocket mySocket;

	int receivedBytes = 0;
	int sentBytes = 0;

	const TorpedoVersion clientVersion;
	//int recShotState=3;
};
