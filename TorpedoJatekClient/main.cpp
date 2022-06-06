#include "TorpedoJatekClient.h"
#include <iostream>
#include <ctime>

int main(int argc, char* args[]) {

	std::cout << "TorpedoJatekClient was compiled on (" << __DATE__
		<< ") with compiler value " << __cplusplus << std::endl;

	//Random számokhoz - Elsõ call nem random
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	std::rand();

	TorpedoJatekClient torpedoJatek;

	return torpedoJatek.Start();

}