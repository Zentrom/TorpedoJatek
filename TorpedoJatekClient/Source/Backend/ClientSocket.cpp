
#include "ClientSocket.h"


ClientSocket::ClientSocket(void)
{
	//SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
}

ClientSocket::~ClientSocket(void)
{
	if (!TorpedoGLOBAL::Debug) {
		SDLNet_TCP_Close(myClient);
	}
	SDLNet_Quit();
	//SDL_Quit();
}

void ClientSocket::Init(std::string ipString,int portNr)
{
	if (SDLNet_ResolveHost(&ip, ipString.c_str(), portNr) == -1) {
		std::cerr << "[SDLNet_ResolveHost] ERROR: " << SDLNet_GetError() << '\n';
	};

	//const char* http = "valami";

	myClient = SDLNet_TCP_Open(&ip);

	if (myClient) {

		//SDLNet_TCP_Send(myClient, http, strlen(http) + 1);

		char text[100];

		receivedBytes = SDLNet_TCP_Recv(myClient, text, 100);
		if (receivedBytes <= 0) {
			std::cerr << "[SDLNet_TCP_Recv] ERROR: " << SDLNet_GetError() << '\n';
		}
		std::cout << text << std::endl;
	}
	else {
		std::cerr << "[SDLNet_TCP_Open] ERROR: " << SDLNet_GetError() << '\n';
	}
	

	
}

void ClientSocket::SendFleet(std::pair<char,int> *activeTiles)
{
	if (myClient) {
		for (int i = 0; i < 16; i++) {
			sentBytes = SDLNet_TCP_Send(myClient, &activeTiles[i], sizeof(std::pair<char,int>));
			if (sentBytes < sizeof(std::pair<char,int>)) {
				std::cerr << "[SDLNet_TCP_Send] ERROR: " << SDLNet_GetError() << '\n';
			}
		}
		receivedBytes = SDLNet_TCP_Recv(myClient, &ServerConfirm, sizeof(int));
		if (receivedBytes <= 0) {
			std::cerr << "[SDLNet_TCP_Recv] ERROR: " << SDLNet_GetError() << '\n';
		}

		if (ServerConfirm) std::cout<<"Server received the ShipData."<<std::endl;
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}
}

int ClientSocket::getPlayerNum()
{
	int playerNum=1;
	if (myClient) {
		SDLNet_TCP_Recv(myClient, &playerNum, sizeof(int));
		std::cout << "You are player number " << playerNum << std::endl;
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}
	return playerNum;
}

int ClientSocket::SendShot(std::pair<char,int> tile)
{
	int stateResult=3;

	if (myClient) {
		sentBytes =  SDLNet_TCP_Send(myClient, &tile, sizeof(std::pair<char,int>));
		if (sentBytes < sizeof(std::pair<char, int>)) {
			std::cerr << "[SDLNet_TCP_Send] ERROR: " << SDLNet_GetError() << '\n';
		}

		receivedBytes = SDLNet_TCP_Recv(myClient, &stateResult, sizeof(int));
		if (receivedBytes <= 0) {
			std::cerr << "[SDLNet_TCP_Recv] ERROR: " << SDLNet_GetError() << '\n';
		}
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}

	return stateResult;
}

std::pair<char,int> ClientSocket::ReceiveShot()
{
	std::pair<char,int> tileNr('0',0);

	if (myClient) {
		receivedBytes = SDLNet_TCP_Recv(myClient, &tileNr, sizeof(std::pair<char,int>));
		if (receivedBytes <= 0) {
			std::cerr << "[SDLNet_TCP_Recv] ERROR: " << SDLNet_GetError() << '\n';
		}
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}

	return tileNr;
}

int ClientSocket::getRecShotState()
{
	int stateResult = 3;

	if (myClient) {
		receivedBytes = SDLNet_TCP_Recv(myClient, &stateResult, sizeof(int));
		if (receivedBytes <= 0) {
			std::cerr << "[SDLNet_TCP_Recv] ERROR: " << SDLNet_GetError() << '\n';
		}
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}

	return stateResult;
}