#pragma once

#include <SDL_net.h>

#include <iostream>
#include <string>

//Hibamentes h�l�zati kommunik�ci�hoz sz�ks�ges static f�ggv�nyek gy�jtem�nye
class ConnectionHandler {
public:
	static void Init_SDLNet();
	static void ResolveHost(IPaddress *address, const char *host, Uint16 port);
	static TCPsocket TCP_Open(IPaddress *ip);

	static void ReceiveText(TCPsocket &socket, void* data, const int length);
	static void ReceiveBinary(TCPsocket &socket, void* dest, const int size);

	static void SendText(TCPsocket &socket, const char *text, const int length);
	static void SendBinary(TCPsocket &socket, const void* source, const int size);

protected:
	//Milyen hibak�d
	enum ErrorCode {
		SDLNET_INIT = 1,
		RESOLVE_HOST,
		OPEN,
		RECEIVE,
		SEND,
	};

	static void printRetry(int currentRetry = 0, char currentAction[] = "Undefined");
	static void ReportErrorAndExit(char functionName[] = "Undefined", int exitCode = 99);

	static const int delayTime = 2000; //�jrapr�b�lkoz�s el�tt mennyit v�rjunk
	static const int maxRetryCountOnError = 3;	//H�nyszor pr�b�ljuk �jra,miel�tt feladjuk

};