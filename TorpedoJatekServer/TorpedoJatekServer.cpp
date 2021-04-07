#include "TorpedoJatekServer.h"

TorpedoJatekServer::TorpedoJatekServer(void)
{
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
	int input = 9999;
	while (setupOption != SetupOptions::CLOSE_PROGRAM && setupOption != SetupOptions::START_SERVER) {
		std::cout << "--------------------------------------------------------------\n"
            << currentSettings.str()
			<< "--------------------------------------------------------------\n"
			<< "Choose an option:\n\
            1.Start server\n\
            2.Change map size\n\
            3.Change port\n\
            0.Quit" << std::endl;
		std::cin >> input;
		setupOption = static_cast<SetupOptions>(input);

		if (setupOption == SetupOptions::CHANGE_MAPSIZE) {
			int tmpMapSize = mapSize;
			do {
				std::cout << "Choose new map-size: 5 OR 7 OR 9" << std::endl;
				std::cin >> tmpMapSize;
			} while (tmpMapSize != 5 && tmpMapSize != 7 && tmpMapSize != 9);
			mapSize = tmpMapSize;
			UpdateSettings();
		}
		if (setupOption == SetupOptions::CHANGE_PORT) {
			std::cout << "New port number: ";
			std::cin >> port;
			UpdateSettings();
		}
	}

	if (setupOption == SetupOptions::START_SERVER) {
		
		CalcActiveTileCount();

		Init();
		GetShips(firstClient);
		GetShips(secondClient);
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

	for (int i = 0; i < tmpCopy.size(); i++) {
		activeTileCount += (i + 1) * tmpCopy[i];
	}
}

//Megnézi,hogy a csatlakozott kliens verziója megegyezik-e a szerverével
bool TorpedoJatekServer::CheckClientVersion(TCPsocket &connectedSocket) {

	const char* text;
	int textLength;

	TorpedoVersion *clientVersion = new TorpedoVersion;
	ServerHandler::ReceiveBinary(connectedSocket, clientVersion, sizeof(TorpedoVersion));

	if (serverVersion.majorVersion == clientVersion->majorVersion && serverVersion.betaVersion == clientVersion->betaVersion
		&& serverVersion.alphaVersion == clientVersion->alphaVersion
		&& serverVersion.experimentalVersion == clientVersion->experimentalVersion) {
		std::cout << "Connected socket passed the version check." << std::endl;
		
		text = "Version check passed.";
		textLength = strlen(text) + 1;
		ServerHandler::SendBinary(connectedSocket, new bool(true), sizeof(bool));
		ServerHandler::SendBinary(connectedSocket, &textLength, sizeof(int));
		ServerHandler::SendText(connectedSocket, text, strlen(text) + 1);
		return true;
	}

	std::stringstream response;
	response << "Version check failed!\n" << "Server version is: v" << serverVersion.majorVersion << '.'
		<< serverVersion.betaVersion << '.' << serverVersion.alphaVersion << '.' << serverVersion.experimentalVersion;
	text = response.str().c_str();
	textLength = strlen(text) + 1;
	ServerHandler::SendBinary(connectedSocket, new bool(false), sizeof(bool));
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

//Azon játékmezõk lekérése a klienstõl,amin van hajó
void TorpedoJatekServer::GetShips(Client &client)
{
	while (successfullyConnectedPlayers<2)
	{
		std::cout << "Waiting for incoming connection..." << std::endl;
		if (ServerHandler::CheckSocket(socketSet, -1)) {
			if (ServerHandler::SocketReady(server)) {
				client.socket = ServerHandler::TCP_Accept(server);
				ServerHandler::TCP_AddSocket(socketSet, client.socket);
			}
			else {
				std::cerr << "Firstclient probably left the server!" << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cin.get();
				std::exit(99);
			}
		}

		if (!CheckClientVersion(client.socket)) {
			std::cerr << "Version mismatch at connected Client!/n";
			SDLNet_TCP_DelSocket(socketSet, client.socket);
			SDLNet_TCP_Close(client.socket);
			continue;
		}

		client.name << "Player" << ++successfullyConnectedPlayers;
		client.playerNum = successfullyConnectedPlayers;
		client.activeTiles.reserve(activeTileCount);

		std::cout << "Sending map size to client..." << std::endl;
		ServerHandler::SendBinary(client.socket, &mapSize, sizeof(int));

		std::cout << "Receiving shipdata from " << client.name.str() << std::endl;
		for (int i = 0; i < activeTileCount; i++) {
			ServerHandler::ReceiveBinary(client.socket, &client.activeTiles[i], sizeof(std::pair<char, int>));
			//std::cout << client.activeTiles[i].first << client.activeTiles[i].second << ' ';
		}
		std::cout << "\nReceived ShipData from " << client.name.str() << std::endl;
		break;
	}
}

//Játékmeccs indítása
void TorpedoJatekServer::StartMatch() {
	int egy = 1;
	int ketto = 2;
	if (firstClient.socket && secondClient.socket) {
		std::cout << "Sending playernumbers..." << std::endl;
		ServerHandler::SendBinary(firstClient.socket, &egy, sizeof(int));
		ServerHandler::SendBinary(secondClient.socket, &ketto, sizeof(int));

		while (firstClient.socket && secondClient.socket && responseState != ResponseState::WIN_PLAYER_ONE
			&& responseState != ResponseState::WIN_PLAYER_TWO) {

			HandleShot(firstClient, secondClient);

			if (firstClient.socket && secondClient.socket && responseState != ResponseState::WIN_PLAYER_ONE
				&& responseState != ResponseState::WIN_PLAYER_TWO) {

				HandleShot(secondClient, firstClient);
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
}

//Egy lövés lekezelése
void TorpedoJatekServer::HandleShot(Client &shooter, Client &taker)
{
	ServerHandler::ReceiveBinary(shooter.socket, &targetTile, sizeof(std::pair<char, int>));

	std::cout << shooter.name.str() << " shot at " << targetTile.first << targetTile.second << std::endl;
	responseState = ProcessTiles(taker);

	ServerHandler::SendBinary(shooter.socket, &responseState, sizeof(ResponseState));
	ServerHandler::SendBinary(taker.socket, &targetTile, sizeof(std::pair<char, int>));
	ServerHandler::SendBinary(taker.socket, &responseState, sizeof(ResponseState));
}

//A célzott játékmezõ lekezelése,és egy válaszállapot visszaadása
TorpedoJatekServer::ResponseState TorpedoJatekServer::ProcessTiles(Client &clientTiles)
{
	ResponseState resultState=ResponseState::CONTINUE_MATCH;
	int allZeros = true;

	//if (playerNum == 1) {
	for (int i = 0; i < activeTileCount; i++) {
		if (targetTile == clientTiles.activeTiles[i]) {
			clientTiles.activeTiles[i] = std::pair<char,int>('0',0);
			resultState = ResponseState::HIT_ENEMY_SHIP;
			break;
		}
	}
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

