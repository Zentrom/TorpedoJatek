#pragma once

#include "../../CommonSource/ConnectionHandler.h"

//Szerverhez köthetõ kapcsolat függvények
class ServerHandler : public ConnectionHandler {
public:
	static void Init_SDL();
	static SDLNet_SocketSet AllocSocketSet(int maxSockets);
	static void TCP_AddSocket(SDLNet_SocketSet set, TCPsocket socket);
	static bool CheckSocket(SDLNet_SocketSet set, Uint32 timeout);
	static bool SocketReady(TCPsocket socket);
	static TCPsocket TCP_Accept(TCPsocket serverSocket);

private:
	//Milyen hibakód
	enum ErrorCode{
		INIT_SDL=10,
		ALLOC_SET,
		CHECK_SOCKETS,
		//SOCKET_READY,
	};
};