#include "ServerHandler.h"

//SDL inicializ�l�sa
void ServerHandler::Init_SDL()
{
	if (SDL_Init(0) == -1) {
		ReportErrorAndExit("SDL_Init", ErrorCode::SDLNET_INIT);
	}
}

//Kapcsolat fogad�sa egy szerversocketen
TCPsocket ServerHandler::TCP_Accept(TCPsocket& server_socket)
{
	return SDLNet_TCP_Accept(server_socket);
}

MessageType ServerHandler::ReceiveMessageType(TCPsocket& socket)
{
	int receivedBytes;
	MessageType result;
	for (int retryCount = 0; retryCount < maxRetryCountOnError; ++retryCount) {
		receivedBytes = SDLNet_TCP_Recv(socket, &result, sizeof(MessageType));
		if (receivedBytes <= 0) {
			printRetry(retryCount, "Receiving message type");
			SDL_Delay(delayTime);
		}
		else {
			return result;
		}
	}
	return MessageType::QUIT;
}

//Nagyobb hiba eset�n kiirjuk �s kil�p�nk
void ServerHandler::ReportErrorAndExit(char function_name[], int exit_code)
{
	std::cerr << "[ERROR] [" << function_name << "] " << SDLNet_GetError() << '\n'
		<< "Press enter to exit...\n";
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
	std::exit(exit_code);
}