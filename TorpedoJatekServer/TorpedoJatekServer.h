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

//Szerver f�oszt�lya
class TorpedoJatekServer
{
public:
	TorpedoJatekServer();
	~TorpedoJatekServer();

	int Run();

private:
	//Melyik szerveropci�
	enum class SetupOptions {
		CLOSE_PROGRAM,
		START_SERVER,
		CHANGE_MAPSIZE,
		CHANGE_PORT,
		DUMMY_OPTION = 9999,
	};

	//A szerver "p�rhuzamos�t�s�hoz" el kell t�rolni,hogy hol tartottunk egy klienssel
	enum class ClientState {
		NOT_CONNECTED,
		RECEIVING_SHIPS,
		IN_MATCH_SHOOTER,
		IN_MATCH_TAKER,
	};

	//Kliensadatok
	struct Client {
		std::stringstream name; //n�v
		int playerNum; //h�nyadik j�t�kos
		TCPsocket socket; //kapcsolati socket
		std::vector<std::pair<char, int>> activeTiles; //Mely mez�koordin�t�in vannak haj�i
		ClientState state = ClientState::NOT_CONNECTED; //Az elej�n a kliens m�g nem csatlakozott
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

	std::array<Client*, 3> clients; //Csatlakozott kliensek pointer t�mbje
	const TorpedoVersion* serverVersion = new TorpedoVersion(); //szerver verzi�sz�ma

	IPaddress ip; //ip oszt�lya
	Uint16 port = 27015; //portsz�m
	SDLNet_SocketSet socketSet = nullptr; //socket csoport
	TCPsocket server = nullptr; //szerver socket
	const int maxSockets = 4; //maximum h�ny socket lehet egy socketcsoportban
	int connectedPlayers = 0; //H�ny j�t�kos csatlakozott - Temp connectiont lesz�m�tva

	std::stringstream currentSettings; //Jelenlegi be�ll�t�sok sz�vegesen
	ResponseState responseState = ResponseState::START_OF_GAME; //Mi a j�t�k �llapota
	MessageType receivedType; //Elv�rt �zenetet k�ld-e a kliens

	std::string input; //Inputot t�rol� string
	int mapSize = 5; //j�t�kP�lya m�rete
	int activeTileCount = 0; //h�ny haj�t tartalmaz� mez� van
	std::pair<char, int> targetTile; //melyik mez�koordin�t�ra l�tt valaki

};
