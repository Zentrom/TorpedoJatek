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

//Ez csak akkor dobhat hibát,ha nem fér el a socketsetben a socket
void ServerHandler::TCP_AddSocket(SDLNet_SocketSet set, TCPsocket socket) 
{
	if (SDLNet_TCP_AddSocket(set, socket) == -1) {
		std::cerr << "SDLNet_TCP_AddSocket" << SDLNet_GetError() << std::endl;
	}
}

bool ServerHandler::CheckSocketReady(SDLNet_SocketSet set,TCPsocket socket, Uint32 timeout)
{
	if (SDLNet_CheckSockets(set, timeout) == -1) {
		perror("SDLNet_CheckSockets");
		ReportErrorAndExit("SDLNet_CheckSockets", ErrorCode::CHECK_SOCKETS);
	}
	else {
		if (SDLNet_SocketReady(socket)) {
			return true;
		}
		else {
			//javitani kell ezt ITT majd
			ReportErrorAndExit("SDLNet_SocketReady", ErrorCode::SOCKET_READY);
			return false;
		}
	}
}

TCPsocket ServerHandler::TCP_Accept(TCPsocket serverSocket)
{
	return SDLNet_TCP_Accept(serverSocket);
}
