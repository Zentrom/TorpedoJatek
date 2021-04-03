#include "ConnectionHandler.h"

void ConnectionHandler::Init_SDLNet()
{
	if (SDLNet_Init() == -1) {
		ReportErrorAndExit("SDLNet_Init", ErrorCode::SDLNET_INIT);
	}
}

void ConnectionHandler::ResolveHost(IPaddress *address, const char *host, Uint16 port)
{
	for (int retryCount = 0; retryCount < maxRetryCountOnError; retryCount++) {
		if (SDLNet_ResolveHost(address, host, port) == -1) {
			printRetry(retryCount, "Resolving address");
			SDL_Delay(delayTime);
		}
		else {
			return;
		}
	}
	ReportErrorAndExit("SDLNet_ResolveHost", ErrorCode::RESOLVE_HOST);
}

TCPsocket ConnectionHandler::TCP_Open(IPaddress *ip)
{
	TCPsocket serverSocket;
	for (int retryCount = 0; retryCount < maxRetryCountOnError; retryCount++) {
		serverSocket = SDLNet_TCP_Open(ip);
		if (!serverSocket) {
			printRetry(retryCount, "Opening socket");
			SDL_Delay(delayTime);
		}
		else {
			return serverSocket;
		}
	}
	ReportErrorAndExit("SDLNet_TCP_Open", ErrorCode::OPEN);
}

void ConnectionHandler::ReceiveText(TCPsocket &socket,void *data, const int length)
{
	//std::string result;
	int receivedBytes;
	for (int retryCount = 0; retryCount < maxRetryCountOnError; retryCount++) {
		receivedBytes = SDLNet_TCP_Recv(socket, data, length);
		if (receivedBytes <= 0) {
			printRetry(retryCount, "Receiving text");
			SDL_Delay(delayTime);
		}
		else {
			return;
		}
	}
	ReportErrorAndExit("SDLNet_TCP_Recv", ErrorCode::RECEIVE);
}

void ConnectionHandler::ReceiveBinary(TCPsocket &socket, void *dest, const int size)
{
	int receivedBytes;
	for (int retryCount = 0; retryCount < maxRetryCountOnError; retryCount++) {
		receivedBytes = SDLNet_TCP_Recv(socket, dest, size);
		if (receivedBytes <= 0) {
			printRetry(retryCount, "Receiving binary data");
			SDL_Delay(delayTime);
		}
		else {
			return;
		}
	}
	ReportErrorAndExit("SDLNet_TCP_Recv", ErrorCode::RECEIVE);
}

void ConnectionHandler::SendText(TCPsocket &socket, const char *text, const int length)
{
	int sentBytes;
	for (int retryCount = 0; retryCount < maxRetryCountOnError; retryCount++) {
		sentBytes = SDLNet_TCP_Send(socket, text, length);
		if (sentBytes < length) {
			printRetry(retryCount, "Sending text");
			SDL_Delay(delayTime);
		}
		else {
			return;
		}
	}
	ReportErrorAndExit("SDLNet_TCP_Send", ErrorCode::SEND);
}

void ConnectionHandler::SendBinary(TCPsocket &socket, const void *source, const int size)
{
	int sentBytes;
	for (int retryCount = 0; retryCount < maxRetryCountOnError;retryCount++){
		sentBytes = SDLNet_TCP_Send(socket, source, size);
		if (sentBytes < size) {
			printRetry(retryCount, "Sending binary data");
			SDL_Delay(delayTime);
		}
		else {
			return;
		}
	}
	ReportErrorAndExit("SDLNet_TCP_Send", ErrorCode::SEND);
}

void ConnectionHandler::printRetry(int currentRetry, char currentAction[]) 
{
	std::cout << "Losing connection while: " << currentAction << '\n'
		<< "Retrying!(" << currentRetry+1 << '/' << maxRetryCountOnError << ')' << std::endl;
}

void ConnectionHandler::ReportErrorAndExit(char functionName[], int exitCode)
{
	std::cerr << "[" << functionName << "] " << SDLNet_GetError() << '\n'
		<< "ERROR OCCURED!Press enter to exit...\n";
	std::cin.get();
	std::cin.get();
	std::exit(exitCode);
}
