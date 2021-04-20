#include "TorpedoJatekServer.h"

TorpedoJatekServer::TorpedoJatekServer(void)
{
	firstClient.playerNum = 1;
	secondClient.playerNum = 2;
	temporaryClient.playerNum = 3;
	clients.at(0) = &firstClient;
	clients.at(1) = &secondClient;
	clients.at(2) = &temporaryClient;

	UpdateSettings();

	ServerHandler::Init_SDL();
	ServerHandler::Init_SDLNet();
}

TorpedoJatekServer::~TorpedoJatekServer(void)
{
	SDLNet_Quit();
	SDL_Quit();
}

//Beállítási szöveg frissítése
void TorpedoJatekServer::UpdateSettings() {
	currentSettings.str("");
	currentSettings << "Server version: " << serverVersion.majorVersion << '.' << serverVersion.betaVersion << '.'
		<< serverVersion.alphaVersion << serverVersion.experimentalVersion << '\n';
	currentSettings << "Current map-size: " << mapSize << '\n';
	currentSettings << "Port used: " << port << '\n';
}

//Elindítja a szervert
int TorpedoJatekServer::Start() {

	SetupOptions setupOption = SetupOptions::DUMMY_OPTION;
	while (setupOption != SetupOptions::CLOSE_PROGRAM && setupOption != SetupOptions::START_SERVER) {
		int intInput = 9999;
		std::cout << "--------------------------------------------------------------\n"
			<< currentSettings.str()
			<< "--------------------------------------------------------------\n"
			<< "Choose an option:\n\
            1.Start server\n\
            2.Change map size\n\
            3.Change port\n\
            0.Quit" << std::endl;
		std::getline(std::cin, input);
		std::stringstream(input) >> intInput;
		setupOption = static_cast<SetupOptions>(intInput);

		if (setupOption == SetupOptions::CHANGE_MAPSIZE) {
			int tmpMapSize = 0;
			do {
				std::cout << "Choose new map-size: 5 OR 7 OR 9" << std::endl;
				std::getline(std::cin, input);
				std::stringstream(input) >> tmpMapSize;
			} while (tmpMapSize != 5 && tmpMapSize != 7 && tmpMapSize != 9);
			mapSize = tmpMapSize;
			UpdateSettings();
		}
		if (setupOption == SetupOptions::CHANGE_PORT) {
			std::cout << "New port number: ";
			std::getline(std::cin, input);
			std::stringstream(input) >> port;
			UpdateSettings();
		}
	}

	if (setupOption == SetupOptions::START_SERVER) {
		CalcActiveTileCount();
		Init();
		PrepareMatch();
		StartMatch();
	}

	return 0;
}

//Kiszámolja a pályaméret alapján,hogy hány játékmezõn lesz hajó
void TorpedoJatekServer::CalcActiveTileCount()
{
	activeTileCount = 0;
	std::array<int, 4> tmpCopy;

	switch (mapSize) {
	case 5:
		tmpCopy = ShipCount::Five;
		break;
	case 7:
		tmpCopy = ShipCount::Seven;
		break;
	case 9:
		tmpCopy = ShipCount::Nine;
		break;
	}

	for (unsigned int i = 0; i < tmpCopy.size(); i++) {
		activeTileCount += (i + 1) * tmpCopy[i];
	}
}

//Megnézi,hogy a csatlakozott kliens verziója megegyezik-e a szerverével
bool TorpedoJatekServer::CheckClientVersion(TCPsocket &connectedSocket) {

	const char *text;
	int textLength;
	bool versionCheckSuccess;

	TorpedoVersion *clientVersion = new TorpedoVersion;
	ServerHandler::ReceiveBinary(connectedSocket, clientVersion, sizeof(TorpedoVersion));

	if (serverVersion.majorVersion == clientVersion->majorVersion && serverVersion.betaVersion == clientVersion->betaVersion
		&& serverVersion.alphaVersion == clientVersion->alphaVersion
		&& serverVersion.experimentalVersion == clientVersion->experimentalVersion) {
		std::cout << "Connected socket passed the version check." << std::endl;

		versionCheckSuccess = true;
		text = "Version check passed.";
		textLength = strlen(text) + 1;
		ServerHandler::SendBinary(connectedSocket, &versionCheckSuccess, sizeof(bool));
		ServerHandler::SendBinary(connectedSocket, &textLength, sizeof(int));
		ServerHandler::SendText(connectedSocket, text, strlen(text) + 1);
		return true;
	}
	delete clientVersion;

	versionCheckSuccess = false;
	std::stringstream response;
	response << "Version check failed!\n" << "Server version is: v" << serverVersion.majorVersion << '.'
		<< serverVersion.betaVersion << '.' << serverVersion.alphaVersion << serverVersion.experimentalVersion;
	std::string convertedTemp = response.str();
	text = convertedTemp.c_str();
	textLength = strlen(text) + 1;
	ServerHandler::SendBinary(connectedSocket, &versionCheckSuccess, sizeof(bool));
	ServerHandler::SendBinary(connectedSocket, &textLength, sizeof(int));
	ServerHandler::SendText(connectedSocket, text, strlen(text) + 1);
	return false;
}

//Szerver létrehozása
void TorpedoJatekServer::Init()
{
	socketSet = ServerHandler::AllocSocketSet(maxSockets);

	ServerHandler::ResolveHost(&ip, nullptr, port);

	std::cout << "Creating server..." << std::endl;
	server = ServerHandler::TCP_Open(&ip);

	ServerHandler::TCP_AddSocket(socketSet, server);
}

//Várunk két kliensre, és elkérjük a mezõket amiken vannak hajóik
void TorpedoJatekServer::PrepareMatch()
{
	while (firstClient.state != ClientState::IN_MATCH_SHOOTER || secondClient.state != ClientState::IN_MATCH_TAKER) {
		std::cout << "Waiting for network activity..." << std::endl;
		if (ServerHandler::CheckSocket(socketSet, static_cast<Uint32>(-1))) {
			if (ServerHandler::SocketReady(server)) {
				HandleClientState(clients.at(getFirstNotConnectedIndex()));
			}
			else if (firstClient.state != ClientState::NOT_CONNECTED && ServerHandler::SocketReady(firstClient.socket)) {
				HandleClientState(clients.at(0));
			}
			else if (secondClient.state != ClientState::NOT_CONNECTED && ServerHandler::SocketReady(secondClient.socket)) {
				HandleClientState(clients.at(1));
			}
		}
	}
	int signalReady = 1;
	ServerHandler::SendBinary(firstClient.socket, &signalReady, sizeof(int));
	ServerHandler::SendBinary(secondClient.socket, &signalReady, sizeof(int));
	std::cout << "Both players ready.Starting match!" << std::endl;
}

//Tovább visszük egy kliens állapotát
void TorpedoJatekServer::HandleClientState(Client *client)
{
	if (client) {
		if (client->state == ClientState::NOT_CONNECTED) {
			client->socket = ServerHandler::TCP_Accept(server);
			ServerHandler::TCP_AddSocket(socketSet, client->socket);

			if (!CheckClientVersion(client->socket)) {
				std::cerr << "Version mismatch at connected Client!\n";
				SDLNet_TCP_DelSocket(socketSet, client->socket);
				SDLNet_TCP_Close(client->socket);
				return;
			}

			client->name.str("");
			client->name << "Player" << client->playerNum;
			if (client->playerNum > 2) {
				ServerHandler::SendBinary(client->socket, &client->playerNum, sizeof(int));
				std::cout << "Server is full.Rejected a connection!" << std::endl;

				SDLNet_TCP_DelSocket(socketSet, client->socket);
				SDLNet_TCP_Close(client->socket);
			}
			else
			{
				client->activeTiles.reserve(activeTileCount);

				ServerHandler::SendBinary(client->socket, &client->playerNum, sizeof(int));

				std::cout << "Sending map size to client..." << std::endl;
				ServerHandler::SendBinary(client->socket, &mapSize, sizeof(int));
				client->state = ClientState::RECEIVING_SHIPS;
				std::cout << client->name.str() << " connected." << '(' << getFirstNotConnectedIndex() << "/2)" << std::endl;
			}
		}
		else if (client->state == ClientState::RECEIVING_SHIPS) {
			receivedType = ServerHandler::ReceiveMessageType(client->socket);
			if (receivedType == MessageType::ESTIMATED) {
				std::cout << "Receiving shipdata from " << client->name.str() << std::endl;
				for (int i = 0; i < activeTileCount; i++) {
					ServerHandler::ReceiveBinary(client->socket, &client->activeTiles[i], sizeof(std::pair<char, int>));
					//std::cout << client->activeTiles[i].first << client->activeTiles[i].second << ' ';
				}
				std::cout << "Received ShipData from " << client->name.str() << std::endl;
				if (client->playerNum == 1) {
					client->state = ClientState::IN_MATCH_SHOOTER;
				}
				else if (client->playerNum == 2) {
					client->state = ClientState::IN_MATCH_TAKER;
				}
			}
			else if (receivedType == MessageType::QUIT) {
				std::cout << client->name.str() << " has left the server!!" << std::endl;
				SDLNet_TCP_DelSocket(socketSet, client->socket);
				SDLNet_TCP_Close(client->socket);
				client->state = ClientState::NOT_CONNECTED;
			}
		}
		else {
			std::cout << "Unwanted socket activity detected(maybe client aborted): ";
			std::cout << client->name.str() << "\nClosing connection." << std::endl;
			SDLNet_TCP_DelSocket(socketSet, firstClient.socket);
			SDLNet_TCP_Close(firstClient.socket);
			firstClient.state = ClientState::NOT_CONNECTED;
		}
	}
	else {
		std::cerr << "Tried to handle a NULL client!\n";
	}
}

//Játékmeccs indítása
void TorpedoJatekServer::StartMatch() {

	while (firstClient.socket && secondClient.socket && responseState != ResponseState::WIN_PLAYER_ONE
		&& responseState != ResponseState::WIN_PLAYER_TWO) {

		if (ServerHandler::CheckSocket(socketSet, static_cast<Uint32>(-1))) {
			if (ServerHandler::SocketReady(server)) {
				HandleClientState(&temporaryClient);
			}
			else if (ServerHandler::SocketReady(firstClient.socket)) {
				HandleShot(firstClient, secondClient);
			}
			else if (ServerHandler::SocketReady(secondClient.socket)) {
				HandleShot(secondClient, firstClient);
			}
		}
	}

	if (responseState == ResponseState::WIN_PLAYER_ONE) {
		std::cout << firstClient.name.str() << " won the match!" << std::endl;
	}
	else if (responseState == ResponseState::WIN_PLAYER_TWO) {
		std::cout << secondClient.name.str() << " won the match!" << std::endl;
	}

	SDLNet_TCP_DelSocket(socketSet, firstClient.socket);
	SDLNet_TCP_DelSocket(socketSet, secondClient.socket);
	SDLNet_TCP_DelSocket(socketSet, server);
	SDLNet_FreeSocketSet(socketSet);

	SDLNet_TCP_Close(firstClient.socket);
	SDLNet_TCP_Close(secondClient.socket);
	SDLNet_TCP_Close(server);

	std::cout << "Press enter to exit..." << std::endl;
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
}

//Egy lövés lekezelése
void TorpedoJatekServer::HandleShot(Client &shooter, Client &taker)
{
	receivedType = ServerHandler::ReceiveMessageType(shooter.socket);
	if (receivedType == MessageType::ESTIMATED) {
		ServerHandler::ReceiveBinary(shooter.socket, &targetTile, sizeof(std::pair<char, int>));

		std::cout << shooter.name.str() << " shot at " << targetTile.first << targetTile.second << std::endl;
		responseState = ProcessTiles(taker);

		ServerHandler::SendBinary(shooter.socket, &responseState, sizeof(ResponseState));
		ServerHandler::SendBinary(taker.socket, &targetTile, sizeof(std::pair<char, int>));
		ServerHandler::SendBinary(taker.socket, &responseState, sizeof(ResponseState));

		shooter.state = ClientState::IN_MATCH_TAKER;
		taker.state = ClientState::IN_MATCH_SHOOTER;
	}
	else if (receivedType == MessageType::QUIT) {
		std::cout << shooter.name.str() << " left the game!!" << std::endl;
		if (shooter.playerNum == 1) {
			responseState = ResponseState::WIN_PLAYER_TWO;
		}
		else if (shooter.playerNum == 2) {
			responseState = ResponseState::WIN_PLAYER_ONE;
		}
		if (shooter.state == ClientState::IN_MATCH_SHOOTER) {
			std::pair<char, int> dummyPair = std::pair<char, int>('0', 0);
			ServerHandler::SendBinary(taker.socket, &dummyPair, sizeof(std::pair<char, int>));
			ServerHandler::SendBinary(taker.socket, &responseState, sizeof(ResponseState));
		}
		//Ez lekezeli ha kilép az aki nem lõ éppen
		else {
			receivedType = ServerHandler::ReceiveMessageType(shooter.socket);
			ServerHandler::ReceiveBinary(taker.socket, &targetTile, sizeof(std::pair<char, int>));
			ServerHandler::SendBinary(taker.socket, &responseState, sizeof(ResponseState));
		}
	}
}

//A célzott játékmezõ lekezelése,és egy válaszállapot visszaadása
ResponseState TorpedoJatekServer::ProcessTiles(Client &clientTiles)
{
	ResponseState resultState = ResponseState::CONTINUE_MATCH;

	//ITT A RANGED-BASED LOOP NEM MEGY,NE PRÓBÁLD.NEMTOM MIÉRT.
	for (int i = 0; i < activeTileCount; i++) {
		if (targetTile == clientTiles.activeTiles[i]) {
			clientTiles.activeTiles[i] = std::pair<char, int>('0', 0);
			resultState = ResponseState::HIT_ENEMY_SHIP;
			break;
		}
	}
	bool allZeros = true;
	for (int i = 0; i < activeTileCount; i++) {
		if (clientTiles.activeTiles[i].first != '0') {
			allZeros = false;
			break;
		}
	}

	if (allZeros) {
		if (clientTiles.playerNum == 1) {
			resultState = ResponseState::WIN_PLAYER_TWO;
		}
		else if (clientTiles.playerNum == 2) {
			resultState = ResponseState::WIN_PLAYER_ONE;
		}
	}

	return resultState;
}

//Visszaadja az elsõ nem csatlakozott kliens indexet
int TorpedoJatekServer::getFirstNotConnectedIndex() const
{
	for (unsigned int i = 0; i < clients.size(); i++) {
		if (clients.at(i)->state == ClientState::NOT_CONNECTED) {
			return i;
		}
	}
	return 0;
}