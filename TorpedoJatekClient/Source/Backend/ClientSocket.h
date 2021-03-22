#pragma once

#include <iostream>
#include <utility>

#include <SDL.h>
#include <SDL_net.h>
#include <cstring>

#include "../Globals.h"

class ClientSocket
{
public:
	ClientSocket(void);
	~ClientSocket(void);

	void Init(std::string ipString,int portNr);
	void SendFleet(std::pair<char,int> *activeTiles);
	int getPlayerNum();
	int SendShot(std::pair<char,int> tile);
	std::pair<char,int> ReceiveShot();

	int getRecShotState();

protected:
	IPaddress ip;
	TCPsocket myClient;

	int receivedBytes = 0;
	int sentBytes = 0;
	int ServerConfirm = 0;

	//int recShotState=3;
};
