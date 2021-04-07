#pragma once

#include <iostream>
#include <string>
#include <SDL_net.h>

//Hibamentes hálózati kommunikációhoz szükséges static függvények gyûjteménye
class ConnectionHandler {
public:
	static void Init_SDLNet();
	static void ResolveHost(IPaddress *address, const char *host, Uint16 port);
	static TCPsocket TCP_Open(IPaddress *ip);

	static void ReceiveText(TCPsocket &socket,void* data,const int length);
	static void ReceiveBinary(TCPsocket &socket, void* dest, const int size);
	
	static void SendText(TCPsocket &socket, const char *text,const int length);
	static void SendBinary(TCPsocket &socket, const void* source, const int size);

protected:
	//Milyen hibakód
	enum ErrorCode{
		SDLNET_INIT = 1,
		RESOLVE_HOST,
		OPEN,
		RECEIVE,
		SEND,
	};

	static void printRetry(int currentRetry, char currentAction[]);
	static void ReportErrorAndExit(char functionName[], int exitCode);

	static const int delayTime = 2000; //Újrapróbálkozás elõtt mennyit várjunk
	static const int maxRetryCountOnError = 5;	//Hányszor próbáljuk újra,mielõtt feladjuk

};