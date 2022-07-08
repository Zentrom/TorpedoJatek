#pragma once

#include "../Globals.hpp"
#include "../../../CommonSource/ConnectionHandler.h"
#include "../../../CommonSource/TorpedoVersion.hpp"
#include "../../../CommonSource/CommonGlobals.hpp"

#include <SDL.h>
#include <SDL_net.h>

#include <iostream>
#include <utility>
#include <cstring>
#include <vector>

//A kliens oldali kapcsolattartó osztály
class ClientHandler : public ConnectionHandler
{
public:
	ClientHandler();
	~ClientHandler();

	bool Init(std::string ipString = "127.0.0.1", int portNr = 27015);
	void SendFleet(const std::vector<std::vector<std::pair<char, int>>> &ship_positions);
	const std::vector<std::vector<std::pair<char, int>>> GetEnemyShipsIfLost();
	int GetMapSize();
	int GetPlayerNum();
	bool GetStartSignal();
	bool CheckForResponse();
	ResponseState SendShot(const std::pair<char, int> &tile);
	const std::pair<char, int> ReceiveShot();
	void QuitGame();
	void CloseConnection();

	ResponseState getRecShotState();

protected:
	IPaddress ip;	//ip-cím
	TCPsocket mySocket;	//kliens oldali socket
	SDLNet_SocketSet socketSet = nullptr; //socket csoport
	ResponseState stateResult = ResponseState::START_OF_GAME; //Mi a játék állapota
	MessageType sentMessageType = MessageType::ESTIMATED; //Szervertõl várt adatot küldünk-e
	
	const int maxSockets = 1; //maximum hány socket lehet egy socketcsoportban

	int receivedBytes = 0;	//szervertõl érkezett byte mennyisége
	int sentBytes = 0; //szervernek átküldött byte mennyisége
	
	const TorpedoVersion clientVersion; //a kliens verziószáma
};
