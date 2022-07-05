#pragma once

#include "Source/ServerHandler.h"
#include "../CommonSource/TorpedoVersion.hpp"
#include "../CommonSource/CommonGlobals.hpp"

#include <SDL.h>
#include <SDL_net.h>

#include <iostream>
#include <cstring>
#include <utility>
#include <vector>
#include <array>
#include <sstream>

//Szerver fõosztálya
class TorpedoJatekServer
{
public:
	TorpedoJatekServer();
	~TorpedoJatekServer();

	int Run();

private:
	//Melyik szerveropció
	enum class SetupOptions {
		CLOSE_PROGRAM,
		START_SERVER,
		CHANGE_MAPSIZE,
		CHANGE_PORT,
		DUMMY_OPTION = 9999,
	};

	//A szerver "párhuzamosításához" el kell tárolni,hogy hol tartottunk egy klienssel
	enum class ClientState {
		NOT_CONNECTED,
		RECEIVING_SHIPS,
		IN_MATCH_SHOOTER,
		IN_MATCH_TAKER,
	};

	//Kliensadatok
	struct Client {
		std::stringstream name; //név
		int playerNum; //hányadik játékos
		TCPsocket socket; //kapcsolati socket
		std::vector<std::pair<char, int>> activeTiles; //Mely mezõkoordinátáin vannak hajói
		ClientState state = ClientState::NOT_CONNECTED; //Az elején a kliens még nem csatlakozott
	}firstClient, secondClient, temporaryClient;

	void Init();
	void DisplaySettings();
	void CalcActiveTileCount();
	void PrepareMatch();
	void HandleClientState(Client& client);
	bool CheckClientVersion(TCPsocket& connected_socket);
	void StartMatch();
	void HandleShot(Client& shooter, Client& taker);
	ResponseState ProcessTiles(Client& client);
	int getFirstNotConnectedIndex() const;

	std::array<Client*, 3> clients; //Csatlakozott kliensek pointer tömbje
	const TorpedoVersion* serverVersion = new TorpedoVersion(); //szerver verziószáma

	IPaddress ip; //ip osztálya
	Uint16 port = 27015; //portszám
	SDLNet_SocketSet socketSet = nullptr; //socket csoport
	TCPsocket server = nullptr; //szerver socket
	const int maxSockets = 4; //maximum hány socket lehet egy socketcsoportban
	int connectedPlayers = 0; //Hány játékos csatlakozott - Temp connectiont leszámítva

	std::stringstream currentSettings; //Jelenlegi beállítások szövegesen
	ResponseState responseState = ResponseState::START_OF_GAME; //Mi a játék állapota
	MessageType receivedType; //Elvárt üzenetet küld-e a kliens

	std::string input; //Inputot tároló string
	int mapSize = 5; //játékPálya mérete
	int activeTileCount = 0; //hány hajót tartalmazó mezõ van
	std::pair<char, int> targetTile; //melyik mezõkoordinátára lõtt valaki

};
