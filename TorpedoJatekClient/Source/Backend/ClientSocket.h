#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <cstring>

class ClientSocket
{
public:
	ClientSocket(void);
	~ClientSocket(void);

	void Init(std::string ipString,int portNr);
	void SendFleet(int* activeTiles);
	int getPlayerNum();
	int SendShot(int tile);
	int ReceiveShot();

	int getRecShotState();

protected:
	IPaddress ip;
	TCPsocket myClient;

	int ServerConfirm=0;

	//int recShotState=3;
};
