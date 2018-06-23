
#include "TorpedoJatekServer.h"


TorpedoJatekServer::TorpedoJatekServer(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
}

TorpedoJatekServer::~TorpedoJatekServer(void)
{
	

	SDLNet_Quit();
	SDL_Quit();

	std::cout << "Press a key to exit..." << std::endl;
	std::cin.get();
}

int TorpedoJatekServer::Start()
{
	SDLNet_ResolveHost(&ip, NULL, 27015);

	server = SDLNet_TCP_Open(&ip);

	const char* text = "HELLO CLIENT!\n";

	//elso jatekos hajoi
	while (1)
	{
		firstClient = SDLNet_TCP_Accept(server);
		if (firstClient)
		{
			//here you can communitcate with the client
			SDLNet_TCP_Send(firstClient, text, strlen(text) + 1);
			
			for (int i = 0; i < 16; i++) {
				SDLNet_TCP_Recv(firstClient, &activeTilesOne[i], sizeof(int));
				//std::cout << this->activeTilesOne[i] << std::endl;
				
			}
			std::cout << "Received ShipData from Player1" << std::endl;
			SDLNet_TCP_Send(firstClient, new int(1), sizeof(int));
			break;
			//closeConnection = !closeConnection;
		}
	}

	//masodik hajoi
	while (1)
	{
		secondClient = SDLNet_TCP_Accept(server);
		if (secondClient)
		{
			//here you can communitcate with the client
			SDLNet_TCP_Send(secondClient, text, strlen(text) + 1);

			for (int i = 0; i < 16; i++) {
				SDLNet_TCP_Recv(secondClient, &activeTilesTwo[i], sizeof(int));
				
				//std::cout << this->activeTilesTwo[i] << std::endl;
			}
			std::cout << "Received ShipData from Player2" << std::endl;
			SDLNet_TCP_Send(secondClient, new int(1), sizeof(int));
			break;
			//closeConnection = !closeConnection;
		}
	}

	int egy = 1;
	int ketto = 2;
	if (firstClient && secondClient) {
		SDLNet_TCP_Send(firstClient, &egy, sizeof(int));
		SDLNet_TCP_Send(secondClient, &ketto, sizeof(int));

		while (firstClient && secondClient && responseState != 4 && responseState != 5) {
			SDLNet_TCP_Recv(firstClient, &targetTile, sizeof(int));
			std::cout << "Player1 shot at " << targetTile << std::endl;
			responseState = ProcessTiles(2);
			SDLNet_TCP_Send(firstClient, &responseState, sizeof(int));
			SDLNet_TCP_Send(secondClient, &targetTile, sizeof(int));
			SDLNet_TCP_Send(secondClient, &responseState, sizeof(int));

			if (firstClient && secondClient && responseState != 4 && responseState != 5) {
				SDLNet_TCP_Recv(secondClient, &targetTile, sizeof(int));
				std::cout << "Player2 shot at " << targetTile << std::endl;
				responseState = ProcessTiles(1);
				SDLNet_TCP_Send(secondClient, &responseState, sizeof(int));
				SDLNet_TCP_Send(firstClient, &targetTile, sizeof(int));
				SDLNet_TCP_Send(firstClient, &responseState, sizeof(int));
			}
		}

		if (responseState == 4) {
			std::cout << "Player1 won the match!" << std::endl;
		}
		else if (responseState == 5) {
			std::cout << "Player2 won the match!" << std::endl;
		}
		else {
			std::cout << "Lost connection!" << std::endl;
		}

		SDLNet_TCP_Close(firstClient);
		SDLNet_TCP_Close(secondClient);
		SDLNet_TCP_Close(server);
	}

	

	return 0;
}

int TorpedoJatekServer::ProcessTiles(int playerNum)
{
	int resultState=2;
	int allZeros = true;

	if (playerNum == 1) {
		for (int i = 0; i < 16; i++) {
			if (targetTile == activeTilesOne[i]) {
				activeTilesOne[i] = 0;
				resultState = 1;
				break;
			}
		}

		for (int i = 0; i < 16; i++) {
			if (activeTilesOne[i] != 0) {
				allZeros = false;
			}
		}

		if (allZeros) {
			resultState = 5;
		}
	}
	else if (playerNum == 2) {
		for (int i = 0; i < 16; i++) {
			if (targetTile == activeTilesTwo[i]) {
				activeTilesTwo[i] = 0;
				resultState = 1;
				break;
			}
		}

		for (int i = 0; i < 16; i++) {
			if (activeTilesTwo[i] != 0) {
				allZeros = false;
			}
		}

		if (allZeros) {
			resultState = 4;
		}
	}

	return resultState;
}


/*
int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	IPaddress ip;
	SDLNet_ResolveHost(&ip, NULL, 1234);

	TCPsocket server = SDLNet_TCP_Open(&ip);
	TCPsocket client;

	const char* text = "HELLO CLIENT!\n";
	while (1)
	{
		client = SDLNet_TCP_Accept(server);
		if (client)
		{
			//here you can communitcate with the client
			SDLNet_TCP_Send(client, text, strlen(text) + 1);
			SDLNet_TCP_Close(client);
			break;
		}
	}
	SDLNet_TCP_Close(server);

	SDLNet_Quit();
	SDL_Quit();
}
*/