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

	void Init();
	void SendFleet(int* activeTiles);

protected:
	IPaddress ip;
	TCPsocket myClient;

	int ServerConfirm=0;
};
