#pragma once

#include "Source\ServerHandler.h"
#include "../CommonSource/TorpedoVersion.h"
#include "../CommonSource/CommonGlobals.h"

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
	TorpedoJatekServer(void);
	~TorpedoJatekServer(void);

	int Start();

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
		int playerNum;	//h�nyadik j�t�kos
		TCPsocket socket;	//kapcsolati socket
		std::vector<std::pair<char, int>> activeTiles;	//Mely mez�koordin�t�in vannak haj�i
		ClientState state = ClientState::NOT_CONNECTED; //Az elej�n a kliens m�g nem csatlakozott
	}firstClient, secondClient, temporaryClient;
	std::array<Client*, 3> clients; //Csatlakozott kliensek pointer t�mbje

	void CalcActiveTileCount();
	void Init();
	void PrepareMatch();
	void HandleClientState(Client *client);
	void StartMatch();
	void UpdateSettings();
	bool CheckClientVersion(TCPsocket &connectedSocket);
	ResponseState ProcessTiles(Client &clientTiles);
	void HandleShot(Client &shooter, Client &taker);
	int getFirstNotConnectedIndex() const;

	IPaddress ip; //ip oszt�lya
	Uint16 port = 27015; //portsz�m
	SDLNet_SocketSet socketSet = nullptr; //socket csoport
	TCPsocket server = nullptr; //szerver socket
	const int maxSockets = 4; //maximum h�ny socket lehet egy socketcsoportban
	//int connectedClients = 0; //h�ny kliens csatlakozott sikeresen

	std::stringstream currentSettings; //Jelenlegi be�ll�t�s sz�vege
	ResponseState responseState = ResponseState::START_OF_GAME; //Mi a j�t�k �llapota
	MessageType receivedType; //Elv�rt �zenetet k�ld-e a kliens

	std::string input; //Inputot t�rol� string
	int mapSize = 7; //j�t�kP�lya m�rete
	int activeTileCount = 0; //h�ny haj�t tartalmaz� mez� van
	std::pair<char, int> targetTile; //melyik mez�koordin�t�ra l�tt valaki

	const TorpedoVersion serverVersion; //szerver verzi�sz�ma

};
