#pragma once

#include <iostream>
#include <string>
#include <SDL_net.h>

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
	enum ErrorCode{
		SDLNET_INIT = 1,
		RESOLVE_HOST,
		OPEN,
		RECEIVE,
		SEND,
	};

	static void printRetry(int currentRetry, char currentAction[]);
	static void ReportErrorAndExit(char functionName[], int exitCode);
	//static int sentBytes;
	//static int receivedBytes;

	static const int delayTime = 2000;
	static const int maxRetryCountOnError = 5;

};