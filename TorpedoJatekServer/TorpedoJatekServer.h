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
	int ProcessTiles(int playerNum);
	IPaddress ip;
	TCPsocket server;
	TCPsocket firstClient;
	TCPsocket secondClient;

	int activeTilesOne[16];
	int activeTilesTwo[16];
	int targetTile=9999;
	int responseState=3;

	bool closeConnection = false;


};
