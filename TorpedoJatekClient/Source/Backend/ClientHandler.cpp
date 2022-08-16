
#include "ClientHandler.h"

ClientHandler::ClientHandler()
{
	Init_SDLNet();
}

ClientHandler::~ClientHandler()
{
	SDLNet_Quit();
}

//Csatlakozik a szerverhez,elk�ldi a kliens verzi�sz�m�t,ki�rja hogy egyezik-e
//Visszaadja,hogy egyeztek-e a verzi�k
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
	//Version check v�lasza
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

//Elk�ri a szervert�l a j�t�kP�lya m�ret�t
int ClientHandler::GetMapSize() {

	std::cout << "Receiving map size from server..." << std::endl;
	int result;
	ReceiveBinary(mySocket, &result, sizeof(int));

	return result;
}

//Elk�ldi a szervernek azokat a j�t�kmez�ket,amin haj�ja van a kliensnek.
void ClientHandler::SendFleet(const std::vector<std::vector<std::pair<char, int>>> &ship_positions)
{
	std::cout << "Sending ships..." << std::endl;
	SendBinary(mySocket, &sentMessageType, sizeof(MessageType));
	std::pair<char, int> data;
	for (const std::vector<std::pair<char, int>> &sh : ship_positions) {
		int shipSize = static_cast<int>(sh.size());
		data.first = 'v';
		data.second = shipSize;
		SendBinary(mySocket, &data, sizeof(std::pair<char, int>));
		for (unsigned int i = 0; i < sh.size(); ++i) {
			SendBinary(mySocket, &sh.at(i), sizeof(std::pair<char, int>));
		}
	}

	std::cout << "ShipData sent to server." << std::endl;
}

//Veszt�s eset�n lek�rj�k a szervert�l az ellenf�l m�g �l� haj�it
const std::vector<std::vector<std::pair<char, int>>> ClientHandler::GetEnemyShipsIfLost()
{
	std::vector<std::vector<std::pair<char, int>>> result;
	std::pair<char, int> data = std::pair<char, int>('0', 0);
	int sentSize = 0;
	while (true) {
		ReceiveBinary(mySocket, &data, sizeof(std::pair<char, int>));
		if (data.first == 'x') break;
		else if (data.first == 'v') {
			sentSize = data.second;
			result.push_back(std::vector<std::pair<char, int>>());
			for (int i = 0; i < sentSize; ++i) {
				ReceiveBinary(mySocket, &data, sizeof(std::pair<char, int>));
				result.back().push_back(data);
			}
		}
	}
	return result;
}

//Lek�ri a szervert�l,hogy mi kezd�nk-e
int ClientHandler::GetPlayerNum()
{
	int playerNum = 1;
	ReceiveBinary(mySocket, &playerNum, sizeof(int));
	if (playerNum > 2) {
		std::cout << "Server is FULL!!\n" << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning", "Server is Full!", nullptr);
	}
	else {
		std::cout << "You are player number " << playerNum << std::endl;
		std::string mBoxText = std::string("You are player number ").append(std::to_string(playerNum));
	}
	return playerNum;
}

//V�r a szerverre,hogy jelezzen,hogy indulhat a meccs.Nem blokkol a CheckSocket miatt
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

//Megn�zi hogy j�n-e �zenet a szervert�l
bool ClientHandler::CheckForResponse()
{
	if (ConnectionHandler::CheckSocket(socketSet, static_cast<Uint32>(0))) {
		if (ConnectionHandler::SocketReady(mySocket)) {
			return true;
		}
	}
	return false;
}

//Elk�ldi a szervernek,hogy mely mez�koordin�t�ra l�v�nk
//Visszaadja a l�v�s�nk eredm�ny�t
ResponseState ClientHandler::SendShot(const std::pair<char, int> &tile)
{
	stateResult = ResponseState::START_OF_GAME;

	SendBinary(mySocket, &sentMessageType, sizeof(MessageType));
	SendBinary(mySocket, &tile, sizeof(std::pair<char, int>));

	ReceiveBinary(mySocket, &stateResult, sizeof(ResponseState));

	return stateResult;
}

//Visszaadja hogy az ellenf�l hova l�tt
const std::pair<char, int> ClientHandler::ReceiveShot()
{
	std::pair<char, int> tileNr('0', 0);

	ReceiveBinary(mySocket, &tileNr, sizeof(std::pair<char, int>));

	return tileNr;
}

//Jelzi a szervernek hogy kil�p�nk
void ClientHandler::QuitGame()
{
	sentMessageType = MessageType::QUIT;
	SendBinary(mySocket, &sentMessageType, sizeof(MessageType));
	CloseConnection();
	std::cout << "You left the game!" << std::endl;
}

void ClientHandler::CloseConnection() 
{
	SDLNet_TCP_DelSocket(socketSet, mySocket);
	SDLNet_FreeSocketSet(socketSet);
	SDLNet_TCP_Close(mySocket);
}

//Megk�rdi a szervert,hogy miut�n kaptunk egy l�v�st,milyen �llapotba lesz a j�t�k
ResponseState ClientHandler::getRecShotState()
{
	stateResult = ResponseState::START_OF_GAME;

	ReceiveBinary(mySocket, &stateResult, sizeof(ResponseState));

	return stateResult;
}