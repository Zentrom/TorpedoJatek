#include "ServerHandler.h"

//SDL inicializálása
void ServerHandler::Init_SDL()
{
	if (SDL_Init(0) == -1) {
		ReportErrorAndExit("SDL_Init", ErrorCode::SDLNET_INIT);
	}
}

//Kapcsolat fogadása egy szerversocketen
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
