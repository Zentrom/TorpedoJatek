#include "ServerHandler.h"

void ServerHandler::Init_SDL()
{
	if (SDL_Init(0) == -1) {
		ReportErrorAndExit("SDL_Init", ErrorCode::INIT_SDL);
	}
}

SDLNet_SocketSet ServerHandler::AllocSocketSet(int maxSockets)
{
	SDLNet_SocketSet socketSet;
	socketSet = SDLNet_AllocSocketSet(maxSockets);
	if (!socketSet) {
		ReportErrorAndExit("SDLNet_AllocSocketSet", ErrorCode::ALLOC_SET);
	}
	return socketSet;
}

//Ez csak akkor dobhat hib�t,ha nem f�r el a socketsetben a socket
void ServerHandler::TCP_AddSocket(SDLNet_SocketSet set, TCPsocket socket) 
{
	if (SDLNet_TCP_AddSocket(set, socket) == -1) {
		std::cerr << "[SDLNet_TCP_AddSocket] " << SDLNet_GetError() << std::endl;
	}
	//ReportErrorAndExit("SDLNet_TCP_AddSocket", 8);
}

//most of the time this is a system error, where perror might help you.
bool ServerHandler::CheckSocketReady(SDLNet_SocketSet set,TCPsocket socket, Uint32 timeout)
{
	if (SDLNet_CheckSockets(set, timeout) == -1) {
		//printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
		perror("SDLNet_CheckSockets");
		ReportErrorAndExit("SDLNet_CheckSockets", ErrorCode::CHECK_SOCKETS);
	}
	else {
		if (SDLNet_SocketReady(socket)) {
			return true;
		}
		else {
			return false;
		}
	}
}

TCPsocket ServerHandler::TCP_Accept(TCPsocket serverSocket)
{
	return SDLNet_TCP_Accept(serverSocket);
}
