#include "TorpedoJatekServer.h"
#include <iostream>

int main(int argc, char* args[]) {

	std::cout << "TorpedoJatekServer was compiled on (" << __DATE__
		<< ") with compiler value " << __cplusplus << std::endl;

	bool quit;
	do {
		TorpedoJatekServer* torpedoServer = new TorpedoJatekServer();
		quit = torpedoServer->Run();
		delete torpedoServer;
	} while (!quit);
	
	return 0;
}