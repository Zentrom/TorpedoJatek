#include "TorpedoJatekClient.h"
#include <iostream>
#include <ctime>

int main(int argc, char* args[]) {

	std::cout << "TorpedoJatekClient was compiled on (" << __DATE__
		<< ") with compiler value " << __cplusplus << std::endl;

	//Random sz�mokhoz - Els� call nem random
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	std::rand();

	TorpedoJatekClient* torpedoJatek = new TorpedoJatekClient();
	int isError = torpedoJatek->Run();
	delete torpedoJatek;

	std::cout << "Press enter to exit..." << std::endl;
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();

	return isError;

}