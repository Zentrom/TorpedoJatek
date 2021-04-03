
#include "ClientHandler.h"


ClientHandler::ClientHandler(void)
{
	//SDL_Init(SDL_INIT_EVERYTHING);
	//SDLNet_Init();
	Init_SDLNet();
}

ClientHandler::~ClientHandler(void)
{
	if (!TorpedoGLOBAL::Debug) {
		SDLNet_TCP_Close(myClient);
	}
	SDLNet_Quit();
	//SDL_Quit();
}

void ClientHandler::Init(std::string ipString,int portNr)
{
	//if (SDLNet_ResolveHost(&ip, ipString.c_str(), portNr) == -1) {
	//	std::cerr << "[SDLNet_ResolveHost] ERROR: " << SDLNet_GetError() << '\n';
	//};
	ResolveHost(&ip, ipString.c_str(), portNr);

	//const char* http = "valami";
	std::cout << "Connecting to server..." << std::endl;
	myClient = TCP_Open(&ip);//SDLNet_TCP_Open(&ip);

	//SDLNet_TCP_Send(myClient, http, strlen(http) + 1);

	char text[100];

	//receivedBytes = SDLNet_TCP_Recv(myClient, text, 100);
	//if (receivedBytes <= 0) {
	//	std::cerr << "[SDLNet_TCP_Recv] ERROR: " << SDLNet_GetError() << '\n';
	//}
	ReceiveText(myClient, text, 100);
	std::cout << text << std::endl;
	
	std::cout << "Sending client version..." << std::endl;
	SendBinary(myClient, &clientVersion, sizeof(TorpedoVersion));
}

void ClientHandler::SendFleet(std::pair<char,int> *activeTiles)
{
	if (myClient) {
		for (int i = 0; i < 16; i++) {
			//sentBytes = SDLNet_TCP_Send(myClient, &activeTiles[i], sizeof(std::pair<char,int>));
			//if (sentBytes < sizeof(std::pair<char,int>)) {
			//	std::cerr << "[SDLNet_TCP_Send] ERROR: " << SDLNet_GetError() << '\n';
			//}
			std::cout << "Sending activetile..." << std::endl;
			SendBinary(myClient, &activeTiles[i], sizeof(std::pair<char, int>));
		}
		//receivedBytes = SDLNet_TCP_Recv(myClient, &ServerConfirm, sizeof(int));
		//if (receivedBytes <= 0) {
		//	std::cerr << "[SDLNet_TCP_Recv] ERROR: " << SDLNet_GetError() << '\n';
		//}
		ReceiveBinary(myClient, &ServerConfirm, sizeof(int));

		if (ServerConfirm) std::cout<<"Server received the ShipData."<<std::endl;
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}
}

int ClientHandler::getPlayerNum()
{
	int playerNum=1;
	if (myClient) {
		//SDLNet_TCP_Recv(myClient, &playerNum, sizeof(int));
		ReceiveBinary(myClient, &playerNum, sizeof(int));
		std::cout << "You are player number " << playerNum << std::endl;
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}
	return playerNum;
}

int ClientHandler::SendShot(std::pair<char,int> tile)
{
	int stateResult=3;

	if (myClient) {
		//sentBytes =  SDLNet_TCP_Send(myClient, &tile, sizeof(std::pair<char,int>));
		//if (sentBytes < sizeof(std::pair<char, int>)) {
		//	std::cerr << "[SDLNet_TCP_Send] ERROR: " << SDLNet_GetError() << '\n';
		//}
		SendBinary(myClient, &tile, sizeof(std::pair<char, int>));

		//receivedBytes = SDLNet_TCP_Recv(myClient, &stateResult, sizeof(int));
		//if (receivedBytes <= 0) {
		//	std::cerr << "[SDLNet_TCP_Recv] ERROR: " << SDLNet_GetError() << '\n';
		//}
		ReceiveBinary(myClient, &stateResult, sizeof(int));
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}

	return stateResult;
}

std::pair<char,int> ClientHandler::ReceiveShot()
{
	std::pair<char,int> tileNr('0',0);

	if (myClient) {
		//receivedBytes = SDLNet_TCP_Recv(myClient, &tileNr, sizeof(std::pair<char,int>));
		//if (receivedBytes <= 0) {
		//	std::cerr << "[SDLNet_TCP_Recv] ERROR: " << SDLNet_GetError() << '\n';
		//}
		ReceiveBinary(myClient, &tileNr, sizeof(std::pair<char, int>));
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}

	return tileNr;
}

int ClientHandler::getRecShotState()
{
	int stateResult = 3;

	if (myClient) {
		//receivedBytes = SDLNet_TCP_Recv(myClient, &stateResult, sizeof(int));
		//if (receivedBytes <= 0) {
		//	std::cerr << "[SDLNet_TCP_Recv] ERROR: " << SDLNet_GetError() << '\n';
		//}
		ReceiveBinary(myClient, &stateResult, sizeof(int));
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}

	return stateResult;
}