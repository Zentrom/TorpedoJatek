#pragma once

#include <SDL_net.h>

#include <iostream>
#include <string>

//Hibamentes h�l�zati kommunik�ci�hoz sz�ks�ges static f�ggv�nyek gy�jtem�nye
class ConnectionHandler {
public:
	static void Init_SDLNet();
	static void ResolveHost(IPaddress* address, const char* host, Uint16 port);
	static TCPsocket TCP_Open(IPaddress* ip);

	static SDLNet_SocketSet AllocSocketSet(int max_sockets = 16);
	static void TCP_AddSocket(SDLNet_SocketSet& set, TCPsocket& socket);
	static bool CheckSocket(SDLNet_SocketSet& set, Uint32 timeout);
	static bool SocketReady(TCPsocket& socket);

	static void ReceiveBinary(TCPsocket& socket, void* dest, const int size);
	static void SendBinary(TCPsocket& socket, const void* source, const int size);

protected:
	//Milyen hibak�d
	enum ErrorCode {
		SDLNET_INIT = 1,
		RESOLVE_HOST,
		ALLOC_SET,
		CHECK_SOCKETS,
		OPEN,
		RECEIVE,
		SEND,
	};

	static void printRetry(int current_retry = 0, char current_action[] = "Undefined");
	static void ReportErrorAndExit(char function_name[] = "Undefined", int exit_code = 99);

	static const int delayTime = 2000; //�jrapr�b�lkoz�s el�tt mennyit v�rjunk
	static const int maxRetryCountOnError = 3;	//H�nyszor pr�b�ljuk �jra,miel�tt feladjuk

};