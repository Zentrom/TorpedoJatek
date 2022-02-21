#pragma once

#include "../../CommonSource/ConnectionHandler.h"
#include "../../CommonSource/CommonGlobals.hpp"

//Szerverhez k�thet� kapcsolat f�ggv�nyek
class ServerHandler : public ConnectionHandler {
public:
	static void Init_SDL();
	static TCPsocket TCP_Accept(TCPsocket &serverSocket);
	static MessageType ReceiveMessageType(TCPsocket &socket);

private:

};