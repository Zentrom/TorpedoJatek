#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <cstring>

class TorpedoJatekServer
{
public:
	TorpedoJatekServer(void);
	~TorpedoJatekServer(void);

	int Start();

protected:
	IPaddress ip;
	TCPsocket server;
	TCPsocket firstClient;
	TCPsocket secondClient;

	int activeTiles[16];
	int targetTile;

	bool closeConnection = false;


};
