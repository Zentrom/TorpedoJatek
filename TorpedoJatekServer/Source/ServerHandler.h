#pragma once

#include "../../CommonSource/ConnectionHandler.h"

class ServerHandler : public ConnectionHandler {
public:
	static void Init_SDL();
	static SDLNet_SocketSet AllocSocketSet(int maxSockets);
	static void TCP_AddSocket(SDLNet_SocketSet set, TCPsocket socket);
	static bool CheckSocketReady(SDLNet_SocketSet set,TCPsocket socket, Uint32 timeout);
	static TCPsocket TCP_Accept(TCPsocket serverSocket);

private:
	enum ErrorCode{
		INIT_SDL=10,
		ALLOC_SET,
		CHECK_SOCKETS,
		SOCKET_READY,
	};
};