
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

	
	while (1)
	{
		firstClient = SDLNet_TCP_Accept(server);
		if (firstClient)
		{
			//here you can communitcate with the client
			SDLNet_TCP_Send(firstClient, text, strlen(text) + 1);
			
			for (int i = 0; i < 16; i++) {
				SDLNet_TCP_Recv(firstClient, &activeTiles[i], sizeof(int));
				std::cout << this->activeTiles[i] << std::endl;
			}

			SDLNet_TCP_Send(firstClient, new int(1), sizeof(int));
			break;
			//closeConnection = !closeConnection;
		}
	}

	SDLNet_TCP_Close(firstClient);

	SDLNet_TCP_Close(server);

	return 0;
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