#include "ConnectionHandler.h"

//SDLNet inicializ�l�s
void ConnectionHandler::Init_SDLNet()
{
	if (SDLNet_Init() == -1) {
		ReportErrorAndExit("SDLNet_Init", ErrorCode::SDLNET_INIT);
	}
}

//Host adatok feldolgoz�sa
void ConnectionHandler::ResolveHost(IPaddress* address, const char* host, Uint16 port)
{
	for (int retryCount = 0; retryCount < maxRetryCountOnError; ++retryCount) {
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

//Csatlakoz�s
TCPsocket ConnectionHandler::TCP_Open(IPaddress* ip)
{
	TCPsocket serverSocket;
	for (int retryCount = 0; retryCount < maxRetryCountOnError; ++retryCount) {
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

	std::cout << "[WARNING] [SDLNet_TCP_Open] Undefined behaviour!! Returning NULL serverSocket!" << std::endl;
	return serverSocket;
}

//Egy socketcsoport foglal�sa a mem�ri�ba
SDLNet_SocketSet ConnectionHandler::AllocSocketSet(int max_sockets)
{
	SDLNet_SocketSet socketSet;
	socketSet = SDLNet_AllocSocketSet(max_sockets);
	if (!socketSet) {
		ReportErrorAndExit("SDLNet_AllocSocketSet", ErrorCode::ALLOC_SET);
	}
	return socketSet;
}

//Socket hozz�ad�sa egy csoporthoz
//Ez csak akkor dobhat hib�t,ha nem f�r el a socketsetben a socket
void ConnectionHandler::TCP_AddSocket(SDLNet_SocketSet& set, TCPsocket& socket)
{
	if (SDLNet_TCP_AddSocket(set, socket) == -1) {
		std::cerr << "[ERROR] SDLNet_TCP_AddSocket" << SDLNet_GetError() << std::endl;
	}
}

//Megn�zi,hogy b�rmely socketen a csoportba,van-e aktivit�s
bool ConnectionHandler::CheckSocket(SDLNet_SocketSet& set, Uint32 timeout)
{
	if (SDLNet_CheckSockets(set, timeout) == -1) {
		perror("SDLNet_CheckSockets");
		ReportErrorAndExit("SDLNet_CheckSockets", ErrorCode::CHECK_SOCKETS);
	}
	return true;
}

//Van-e aktivit�s egy socketen
bool ConnectionHandler::SocketReady(TCPsocket& socket)
{
	if (SDLNet_SocketReady(socket)) {
		return true;
	}
	else {
		return false;
	}
}

//Bin�ris adat fogad�sa h�l�zaton
void ConnectionHandler::ReceiveBinary(TCPsocket& socket, void* dest, const int size)
{
	int receivedBytes;
	for (int retryCount = 0; retryCount < maxRetryCountOnError; ++retryCount) {
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

//Bin�ris adat k�ld�se h�l�zaton
void ConnectionHandler::SendBinary(TCPsocket& socket, const void* source, const int size)
{
	int sentBytes;
	for (int retryCount = 0; retryCount < maxRetryCountOnError; ++retryCount) {
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

//Kiirjuk hogy �jra pr�b�lkozunk
void ConnectionHandler::printRetry(int current_retry, char current_action[])
{
	std::cout << "[WARNING] Losing connection while: " << current_action << '\n'
		<< "Retrying!(" << current_retry + 1 << '/' << maxRetryCountOnError << ')' << std::endl;
}

//Nagyobb hiba eset�n kiirjuk �s kil�p�nk
void ConnectionHandler::ReportErrorAndExit(char function_name[], int exit_code)
{
	std::cerr << "[ERROR] [" << function_name << "] " << SDLNet_GetError() << '\n'
		<< "Press enter to exit...\n";
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
	std::exit(exit_code);
}
