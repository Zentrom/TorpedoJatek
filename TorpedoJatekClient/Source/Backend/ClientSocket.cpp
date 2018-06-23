
#include "ClientSocket.h"


ClientSocket::ClientSocket(void)
{
	//SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
}

ClientSocket::~ClientSocket(void)
{
	SDLNet_TCP_Close(myClient);
	SDLNet_Quit();
	//SDL_Quit();
}

void ClientSocket::Init(std::string ipString,int portNr)
{
	SDLNet_ResolveHost(&ip, ipString.c_str(), portNr);

	const char* http = "valami";

	myClient = SDLNet_TCP_Open(&ip);

	if (myClient != NULL) {

		//SDLNet_TCP_Send(myClient, http, strlen(http) + 1);

		char text[100];

		SDLNet_TCP_Recv(myClient, text, 100);
		std::cout << text << std::endl;
	}
	else {
		std::cout << "Server not found!" << std::endl;
	}
	

	
}

void ClientSocket::SendFleet(int * activeTiles)
{
	if (myClient != NULL) {
		for (int i = 0; i < 16; i++) {
			SDLNet_TCP_Send(myClient, &activeTiles[i], sizeof(int));

		}
		SDLNet_TCP_Recv(myClient, &ServerConfirm, sizeof(int));

		if (ServerConfirm) std::cout<<"Server received the ShipData."<<std::endl;
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}
}

int ClientSocket::getPlayerNum()
{
	int playerNum=1;
	if (myClient != NULL) {
		SDLNet_TCP_Recv(myClient, &playerNum, sizeof(int));
		std::cout << "You are player number " << playerNum << std::endl;
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}
	return playerNum;
}

int ClientSocket::SendShot(int tile)
{
	int stateResult=3;

	if (myClient != NULL) {
		SDLNet_TCP_Send(myClient, &tile, sizeof(int));
		//std::cout << "You are player number " << playerNum << std::endl;
		SDLNet_TCP_Recv(myClient, &stateResult, sizeof(int));
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}

	return stateResult;
}

int ClientSocket::ReceiveShot()
{
	int tileNr=1;

	if (myClient != NULL) {

		SDLNet_TCP_Recv(myClient, &tileNr, sizeof(int));
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}

	return tileNr;
}

int ClientSocket::getRecShotState()
{
	int stateResult = 3;

	if (myClient != NULL) {
		
		SDLNet_TCP_Recv(myClient, &stateResult, sizeof(int));
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}

	return stateResult;
}

/*

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	IPaddress ip;
	//write "127.0.0.1",1234 to connect to the server.cpp on your local machine
	SDLNet_ResolveHost(&ip, "www.linux.org", 80);

	const char* http = "GET / HTTP/1.1\nHost: www.linux.org\n\n";

	TCPsocket client = SDLNet_TCP_Open(&ip);

	SDLNet_TCP_Send(client, http, strlen(http) + 1);

	char text[10000];

	while (SDLNet_TCP_Recv(client, text, 10000))
		std::cout << text;

	SDLNet_TCP_Close(client);

	SDLNet_Quit();
	SDL_Quit();
}

*/