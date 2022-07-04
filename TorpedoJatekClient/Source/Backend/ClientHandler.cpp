
#include "ClientHandler.h"

ClientHandler::ClientHandler()
{
	Init_SDLNet();
}

ClientHandler::~ClientHandler()
{
	if (!TorpedoGLOBAL::Debug) {
		SDLNet_TCP_Close(mySocket);
	}
	SDLNet_Quit();
}

//Csatlakozik a szerverhez,elküldi a kliens verziószámát,kiírja hogy egyezik-e
//Visszaadja,hogy egyeztek-e a verziók
bool ClientHandler::Init(std::string ipString, int portNr)
{
	socketSet = ConnectionHandler::AllocSocketSet(maxSockets);

	std::cout << "Connecting to server..." << std::endl;
	ResolveHost(&ip, ipString.c_str(), static_cast<Uint16>(portNr));

	mySocket = TCP_Open(&ip);
	std::cout << "Connected!" << std::endl;

	ConnectionHandler::TCP_AddSocket(socketSet, mySocket);

	std::cout << "Sending client version..." << std::endl;
	SendBinary(mySocket, &clientVersion, sizeof(TorpedoVersion));
	//Version check válasza
	char text[200];
	int textLength;
	bool responseVersionCheck = false;
	ReceiveBinary(mySocket, &responseVersionCheck, sizeof(bool));
	ReceiveBinary(mySocket, &textLength, sizeof(int));
	ReceiveBinary(mySocket, text, textLength); //char = 1byte
	text[textLength] = '\0';
	std::cout << text << std::endl;

	return responseVersionCheck;
}

//Elkéri a szervertõl a játékPálya méretét
int ClientHandler::GetMapSize() {

	std::cout << "Receiving map size from server..." << std::endl;
	int result;
	ReceiveBinary(mySocket, &result, sizeof(int));

	return result;
}

//Elküldi a szervernek azokat a játékmezõket,amin hajója van a kliensnek.
void ClientHandler::SendFleet(const std::vector<std::pair<char, int>> &activeTilePositions)
{
	std::cout << "Sending activetiles..." << std::endl;
	SendBinary(mySocket, &sentMessageType, sizeof(MessageType));
	for (std::pair<char, int> activeTilePos : activeTilePositions) {
		SendBinary(mySocket, &activeTilePos, sizeof(std::pair<char, int>));
	}
	std::cout << "ShipData sent to server." << std::endl;
}

//Lekéri a szervertõl,hogy mi kezdünk-e
int ClientHandler::GetPlayerNum()
{
	int playerNum = 1;
	ReceiveBinary(mySocket, &playerNum, sizeof(int));
	if (playerNum > 2) {
		std::cout << "Server is FULL!!\n" << std::endl;
	}
	else {
		std::cout << "You are player number " << playerNum << std::endl;
	}
	return playerNum;
}

//Vár a szerverre,hogy jelezzen,hogy indulhat a meccs.Nem blokkol a CheckSocket miatt
bool ClientHandler::GetStartSignal()
{
	int startSignal;
	if (ConnectionHandler::CheckSocket(socketSet, static_cast<Uint32>(0))) {
		if (ConnectionHandler::SocketReady(mySocket)) {
			ReceiveBinary(mySocket, &startSignal, sizeof(int));
			std::cout << "Match Started!" << std::endl;
			return true;
		}
	}
	return false;
}

//Megnézi hogy jön-e üzenet a szervertõl
bool ClientHandler::CheckForResponse()
{
	if (ConnectionHandler::CheckSocket(socketSet, static_cast<Uint32>(0))) {
		if (ConnectionHandler::SocketReady(mySocket)) {
			return true;
		}
	}
	return false;
}

//Elküldi a szervernek,hogy mely mezõkoordinátára lövünk
//Visszaadja a lövésünk eredményét
ResponseState ClientHandler::SendShot(const std::pair<char, int> &tile)
{
	stateResult = ResponseState::START_OF_GAME;

	SendBinary(mySocket, &sentMessageType, sizeof(MessageType));
	SendBinary(mySocket, &tile, sizeof(std::pair<char, int>));

	ReceiveBinary(mySocket, &stateResult, sizeof(ResponseState));

	return stateResult;
}

//Visszaadja hogy az ellenfél hova lõtt
std::pair<char, int> ClientHandler::ReceiveShot()
{
	std::pair<char, int> tileNr('0', 0);

	ReceiveBinary(mySocket, &tileNr, sizeof(std::pair<char, int>));

	return tileNr;
}

//Jelzi a szervernek hogy kilépünk
void ClientHandler::quitGame()
{
	sentMessageType = MessageType::QUIT;
	SendBinary(mySocket, &sentMessageType, sizeof(MessageType));

	std::cout << "You left the game!" << std::endl;
}

//Megkérdi a szervert,hogy miután kaptunk egy lövést,milyen állapotba lesz a játék
ResponseState ClientHandler::getRecShotState()
{
	stateResult = ResponseState::START_OF_GAME;

	ReceiveBinary(mySocket, &stateResult, sizeof(ResponseState));

	return stateResult;
}