#include "TorpedoJatekClient.h"
#include <iostream>

int main(int argc, char* args[]) {

	std::cout << "TorpedoJatekClient was compiled on (" << __DATE__
		<< ") with compiler value " << __cplusplus << std::endl;

	TorpedoJatekClient torpedoJatek;

	return torpedoJatek.Start();

}