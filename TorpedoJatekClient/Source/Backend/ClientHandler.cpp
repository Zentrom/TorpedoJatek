
#include "ClientHandler.h"


ClientHandler::ClientHandler(void)
{
	Init_SDLNet();
}

ClientHandler::~ClientHandler(void)
{
	if (!TorpedoGLOBAL::Debug) {
		SDLNet_TCP_Close(mySocket);
	}
	SDLNet_Quit();
}

//Csatlakozik a szerverhez,elk�ldi a kliens verzi�sz�m�t,ki�rja hogy egyezik-e
//Visszaadja,hogy egyeztek-e a verzi�k
bool ClientHandler::Init(std::string ipString,int portNr)
{
	ResolveHost(&ip, ipString.c_str(), portNr);

	std::cout << "Connecting to server..." << std::endl;
	mySocket = TCP_Open(&ip);
	std::cout << "Connected!" << std::endl;
	
	std::cout << "Sending client version..." << std::endl;
	SendBinary(mySocket, &clientVersion, sizeof(TorpedoVersion));

	char text[200];
	int textLength;
	bool responseVersionCheck = false;
	ReceiveBinary(mySocket, &responseVersionCheck, sizeof(bool));
	ReceiveBinary(mySocket, &textLength, sizeof(int));
	ReceiveText(mySocket, text, textLength);
	text[textLength] = '\0';
	std::cout << text << std::endl;

	return responseVersionCheck;
}

//Elk�ri a szervert�l a j�t�kP�lya m�ret�t
int ClientHandler::getMapSize() {

	std::cout << "Receiving map size from server..." << std::endl;
	int result;
	ReceiveBinary(mySocket, &result, sizeof(int));

	return result;
}

//Elk�ldi a szervernek azokat a j�t�kmez�ket,amin haj�ja van a kliensnek.
void ClientHandler::SendFleet(std::vector<std::pair<char,int>> activeTilePositions)
{
	std::cout << "Sending activetiles..." << std::endl;
	if (mySocket) {
		for (int i = 0; i < activeTilePositions.size(); i++) {
			SendBinary(mySocket, &activeTilePositions[i], sizeof(std::pair<char, int>));
		}
		std::cout<<"ShipData sent to server."<<std::endl;
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}
}

//Lek�ri a szervert�l,hogy mi kezd�nk-e
int ClientHandler::getPlayerNum()
{
	int playerNum=1;
	if (mySocket) {
		ReceiveBinary(mySocket, &playerNum, sizeof(int));
		std::cout << "You are player number " << playerNum << std::endl;
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}
	return playerNum;
}

//Elk�ldi a szervernek,hogy mely mez�koordin�t�ra l�v�nk
//Visszaadja a l�v�s�nk eredm�ny�t
int ClientHandler::SendShot(std::pair<char,int> tile)
{
	int stateResult=3;

	if (mySocket) {
		SendBinary(mySocket, &tile, sizeof(std::pair<char, int>));

		ReceiveBinary(mySocket, &stateResult, sizeof(int));
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}

	return stateResult;
}

//Visszaadja hogy az ellenf�l hova l�tt
std::pair<char,int> ClientHandler::ReceiveShot()
{
	std::pair<char,int> tileNr('0',0);

	if (mySocket) {
		ReceiveBinary(mySocket, &tileNr, sizeof(std::pair<char, int>));
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}

	return tileNr;
}

//Megk�rdi a szervert,hogy miut�n kaptunk egy l�v�st,milyen �llapotba lesz a j�t�k
int ClientHandler::getRecShotState()
{
	int stateResult = 3;

	if (mySocket) {
		ReceiveBinary(mySocket, &stateResult, sizeof(int));
	}
	else {
		std::cout << "No connection to the server!" << std::endl;
	}

	return stateResult;
}