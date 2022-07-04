#include "TorpedoJatekServer.h"
#include <iostream>

int main(int argc, char* args[]) {

	std::cout << "TorpedoJatekServer was compiled on (" << __DATE__
		<< ") with compiler value " << __cplusplus << std::endl;

	TorpedoJatekServer* torpedoServer = new TorpedoJatekServer();

	int isError = torpedoServer->Run();
	delete torpedoServer;

	return isError;
}