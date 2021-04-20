#include "ServerHandler.h"

//SDL inicializálása
void ServerHandler::Init_SDL()
{
	if (SDL_Init(0) == -1) {
		ReportErrorAndExit("SDL_Init", ErrorCode::INIT_SDL);
	}
}

//Egy socketcsoport foglalása a memóriába
SDLNet_SocketSet ServerHandler::AllocSocketSet(int maxSockets)
{
	SDLNet_SocketSet socketSet;
	socketSet = SDLNet_AllocSocketSet(maxSockets);
	if (!socketSet) {
		ReportErrorAndExit("SDLNet_AllocSocketSet", ErrorCode::ALLOC_SET);
	}
	return socketSet;
}

//Socket hozzáadása egy csoporthoz
//Ez csak akkor dobhat hibát,ha nem fér el a socketsetben a socket
void ServerHandler::TCP_AddSocket(SDLNet_SocketSet &set, TCPsocket &socket)
{
	if (SDLNet_TCP_AddSocket(set, socket) == -1) {
		std::cerr << "SDLNet_TCP_AddSocket" << SDLNet_GetError() << std::endl;
	}
}

//Megnézi,hogy bármely socketen a csoportba,van-e aktivitás
bool ServerHandler::CheckSocket(SDLNet_SocketSet &set, Uint32 timeout)
{
	if (SDLNet_CheckSockets(set, timeout) == -1) {
		perror("SDLNet_CheckSockets");
		ReportErrorAndExit("SDLNet_CheckSockets", ErrorCode::CHECK_SOCKETS);
	}
	else {
		return true;
	}
}

//Van-e aktivitás egy socketen
bool ServerHandler::SocketReady(TCPsocket &socket)
{
	if (SDLNet_SocketReady(socket)) {
		return true;
	}
	else {
		//ReportErrorAndExit("SDLNet_SocketReady", ErrorCode::SOCKET_READY);
		return false;
	}
}

//Kapcsolat fogadása egy szerversocketen
TCPsocket ServerHandler::TCP_Accept(TCPsocket &serverSocket)
{
	return SDLNet_TCP_Accept(serverSocket);
}

MessageType ServerHandler::ReceiveMessageType(TCPsocket &socket)
{
	int receivedBytes;
	MessageType result;
	for (int retryCount = 0; retryCount < maxRetryCountOnError; retryCount++) {
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
