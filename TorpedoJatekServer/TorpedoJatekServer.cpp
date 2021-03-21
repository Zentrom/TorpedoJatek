#include "TorpedoJatekServer.h"

TorpedoJatekServer::TorpedoJatekServer(void)
{
	UpdateSettings();

	if (SDL_Init(0) == -1) {
		ReportErrorAndExit("SDL_Init", 1);
	}
	if (SDLNet_Init() == -1) {
		ReportErrorAndExit("SDLNet_Init", 2);
	}
}

TorpedoJatekServer::~TorpedoJatekServer(void)
{
	SDLNet_Quit();
	SDL_Quit();

	//std::cout << "Press enter to exit..." << std::endl;
	//std::cin.get();
}

void TorpedoJatekServer::UpdateSettings() {
	currentSettings.str("");
	currentSettings << "Server version: " << serverVersion.majorVersion << '.' << serverVersion.betaVersion << '.'
		<< serverVersion.alphaVersion << serverVersion.experimentalVersion << '\n';
	currentSettings << "Current map-size: " << mapSize << '\n';
}

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
	}

	if (setupOption == SetupOptions::START_SERVER) {
		Init();
		StartMatch();
	}

	return 0;
}

bool TorpedoJatekServer::CheckClientVersion(TCPsocket &connectedSocket) {
	TorpedoVersion *clientVersion = new TorpedoVersion;

	receivedBytes = SDLNet_TCP_Recv(connectedSocket, clientVersion, sizeof(TorpedoVersion));
	if (receivedBytes <= 0) {
		delete clientVersion;
		ReportErrorAndExit("SDLNet_TCP_Recv", 6);
	}

	if (serverVersion.majorVersion == clientVersion->majorVersion && serverVersion.betaVersion == clientVersion->betaVersion
		&& serverVersion.alphaVersion == clientVersion->alphaVersion
		&& serverVersion.experimentalVersion == clientVersion->experimentalVersion) {
		std::cout << "connectedSocket passed the version check." << std::endl;
		return true;
	}

	return false;
}

void TorpedoJatekServer::Init()
{
	socketSet = SDLNet_AllocSocketSet(maxSockets);
	if (!socketSet) {
		ReportErrorAndExit("SDLNet_AllocSocketSet", 7);
	}

	if (SDLNet_ResolveHost(&ip, nullptr, port) == -1) {
		ReportErrorAndExit("SDLNet_ResolveHost", 3);
	};

	server = SDLNet_TCP_Open(&ip);
	if (!server) {
		ReportErrorAndExit("SDLNet_TCP_Open", 4);
	}
	if (SDLNet_TCP_AddSocket(socketSet, server) == -1) {
		ReportErrorAndExit("SDLNet_TCP_AddSocket", 8);
	}

	const char* text = "HELLO CLIENT!\n";

	//elso jatekos hajoi
	while (true)
	{
		if (SDLNet_CheckSockets(socketSet, -1) == -1) {
			printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
			//most of the time this is a system error, where perror might help you.
			perror("SDLNet_CheckSockets");
		}else{
			if (SDLNet_SocketReady(server)) {
				firstClient = SDLNet_TCP_Accept(server);
				if (SDLNet_TCP_AddSocket(socketSet, firstClient) == -1) {
					ReportErrorAndExit("SDLNet_TCP_AddSocket", 8);
				}
			}
		}

		if (firstClient)
		{
			firstClientAddress = SDLNet_TCP_GetPeerAddress(firstClient);
			if (!firstClientAddress) {
				std::cout << "[SDLNet_TCP_GetPeerAddress] " << SDL_GetError() << std::endl;
			}
			else {
				std::cout << "Player1 connected: " << firstClientAddress->host << " : "
					<< firstClientAddress->port << std::endl;
			}

			sentBytes = SDLNet_TCP_Send(firstClient, text, strlen(text) + 1);
			if (sentBytes < (strlen(text) + 1)) {
				ReportErrorAndExit("SDLNet_TCP_Send", 5);
			}
			if (!CheckClientVersion(firstClient)) {
				std::cerr << "Version mismatch at firstClient!/n";
				std::exit(9);
			}

			for (int i = 0; i < 16; i++) {
				receivedBytes = SDLNet_TCP_Recv(firstClient, &activeTilesPlayerOne[i], sizeof(std::pair<char,int>));
				if (receivedBytes <= 0) {
					ReportErrorAndExit("SDLNet_TCP_Recv", 6);
				}
				else {
					std::cout << activeTilesPlayerOne[i].first << activeTilesPlayerOne[i].second << std::endl;
				}
			}
			std::cout << "Received ShipData from Player1" << std::endl;
			sentBytes = SDLNet_TCP_Send(firstClient, new int(1), sizeof(int));
			if (sentBytes < sizeof(int)) {
				ReportErrorAndExit("SDLNet_TCP_Send", 5);
			}
			break;
			//closeConnection = !closeConnection;
		}
		else {
			std::cout << SDL_GetError() << std::endl;
		}
	}

	//masodik hajoi
	while (true)
	{
		if (SDLNet_CheckSockets(socketSet, -1) == -1) {
			printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
			//most of the time this is a system error, where perror might help you.
			perror("SDLNet_CheckSockets");
		}
		else {
			if (SDLNet_SocketReady(server)) {
				secondClient = SDLNet_TCP_Accept(server);
				if (SDLNet_TCP_AddSocket(socketSet, secondClient) == -1) {
					ReportErrorAndExit("SDLNet_TCP_AddSocket", 8);
				}
			}
		}

		if (secondClient)
		{
			secondClientAddress = SDLNet_TCP_GetPeerAddress(secondClient);
			if (!secondClientAddress) {
				std::cout << "[SDLNet_TCP_GetPeerAddress] " << SDL_GetError() << std::endl;
			}
			else {
				std::cout << "Player2 connected: " << secondClientAddress->host << " : "
					<< secondClientAddress->port << std::endl;
			}

			sentBytes = SDLNet_TCP_Send(secondClient, text, strlen(text) + 1);
			if (sentBytes < (strlen(text) + 1)) {
				ReportErrorAndExit("SDLNet_TCP_Send", 5);
			}
			if (!CheckClientVersion(secondClient)) {
				std::cerr << "Version mismatch at secondClient!/n";
				std::exit(9);
			}

			for (int i = 0; i < 16; i++) {
				receivedBytes = SDLNet_TCP_Recv(secondClient, &activeTilesPlayerTwo[i], sizeof(std::pair<char, int>));
				if (receivedBytes <= 0) {
					ReportErrorAndExit("SDLNet_TCP_Recv", 6);
				}
				else {
					std::cout << activeTilesPlayerTwo[i].first << activeTilesPlayerTwo[i].second << std::endl;
				}
			}
			std::cout << "Received ShipData from Player2" << std::endl;
			sentBytes = SDLNet_TCP_Send(secondClient, new int(1), sizeof(int));
			if (sentBytes < sizeof(int)) {
				ReportErrorAndExit("SDLNet_TCP_Send", 5);
			}
			break;
			//closeConnection = !closeConnection;
		}
		else {
			std::cout << SDL_GetError() << std::endl;
		}
	}
}

void TorpedoJatekServer::StartMatch() {
	int egy = 1;
	int ketto = 2;
	if (firstClient && secondClient) {
		sentBytes = SDLNet_TCP_Send(firstClient, &egy, sizeof(int));
		if (sentBytes < sizeof(int)) {
			ReportErrorAndExit("SDLNet_TCP_Send", 5);
		}
		sentBytes = SDLNet_TCP_Send(secondClient, &ketto, sizeof(int));
		if (sentBytes < sizeof(int)) {
			ReportErrorAndExit("SDLNet_TCP_Send", 5);
		}

		while (firstClient && secondClient && responseState != ResponseState::WIN_PLAYER_ONE
			&& responseState != ResponseState::WIN_PLAYER_TWO) {

			receivedBytes = SDLNet_TCP_Recv(firstClient, &targetTile, sizeof(std::pair<char, int>));
			if (receivedBytes <= 0) {
				ReportErrorAndExit("SDLNet_TCP_Recv", 6);
			}

			std::cout << "Player1 shot at " << targetTile.first << targetTile.second << std::endl;
			responseState = ProcessTiles(2);

			sentBytes = SDLNet_TCP_Send(firstClient, &responseState, sizeof(ResponseState));
			if (sentBytes < sizeof(ResponseState)) {
				ReportErrorAndExit("SDLNet_TCP_Send", 5);
			}
			sentBytes = SDLNet_TCP_Send(secondClient, &targetTile, sizeof(std::pair<char, int>));
			if (sentBytes < sizeof(std::pair<char,int>)) {
				ReportErrorAndExit("SDLNet_TCP_Send", 5);
			}
			sentBytes = SDLNet_TCP_Send(secondClient, &responseState, sizeof(ResponseState));
			if (sentBytes < sizeof(ResponseState)) {
				ReportErrorAndExit("SDLNet_TCP_Send", 5);
			}

			if (firstClient && secondClient && responseState != ResponseState::WIN_PLAYER_ONE
				&& responseState != ResponseState::WIN_PLAYER_TWO) {

				receivedBytes = SDLNet_TCP_Recv(secondClient, &targetTile, sizeof(std::pair<char, int>));
				if (receivedBytes <= 0) {
					ReportErrorAndExit("SDLNet_TCP_Recv", 6);
				}

				std::cout << "Player2 shot at " << targetTile.first << targetTile.second << std::endl;
				responseState = ProcessTiles(1);

				sentBytes = SDLNet_TCP_Send(secondClient, &responseState, sizeof(ResponseState));
				if (sentBytes < sizeof(ResponseState)) {
					ReportErrorAndExit("SDLNet_TCP_Send", 5);
				}
				sentBytes = SDLNet_TCP_Send(firstClient, &targetTile, sizeof(std::pair<char, int>));
				if (sentBytes < sizeof(std::pair<char, int>)) {
					ReportErrorAndExit("SDLNet_TCP_Send", 5);
				}
				sentBytes = SDLNet_TCP_Send(firstClient, &responseState, sizeof(ResponseState));
				if (sentBytes < sizeof(ResponseState)) {
					ReportErrorAndExit("SDLNet_TCP_Send", 5);
				}
			}
		}

		if (responseState == ResponseState::WIN_PLAYER_ONE) {
			std::cout << "Player1 won the match!" << std::endl;
		}
		else if (responseState == ResponseState::WIN_PLAYER_TWO) {
			std::cout << "Player2 won the match!" << std::endl;
		}

		SDLNet_TCP_DelSocket(socketSet, firstClient);
		SDLNet_TCP_DelSocket(socketSet, secondClient);
		SDLNet_TCP_DelSocket(socketSet, server);
		SDLNet_FreeSocketSet(socketSet);

		SDLNet_TCP_Close(firstClient);
		SDLNet_TCP_Close(secondClient);
		SDLNet_TCP_Close(server);
	}
}

void TorpedoJatekServer::ReportErrorAndExit(char functionName[], int exitCode)
{
	std::cerr << "[" << functionName << "] " << SDL_GetError() << '\n'
		<< "ERROR OCCURED!Press enter to exit...\n";
	int getlinePlaceholder;
	std::cin >> getlinePlaceholder;
	std::exit(exitCode);
}

TorpedoJatekServer::ResponseState TorpedoJatekServer::ProcessTiles(int playerNum)
{
	ResponseState resultState=ResponseState::CONTINUE_MATCH;
	int allZeros = true;

	if (playerNum == 1) {
		for (int i = 0; i < 16; i++) {
			if (targetTile == activeTilesPlayerOne[i]) {
				activeTilesPlayerOne[i] = std::pair<char,int>('0',0);
				resultState = ResponseState::HIT_ENEMY_SHIP;
				break;
			}
		}
		for (int i = 0; i < 16; i++) {
			if (activeTilesPlayerOne[i].first != '0') {
				allZeros = false;
			}
		}
		if (allZeros) {
			resultState = ResponseState::WIN_PLAYER_TWO;
		}
	}
	else if (playerNum == 2) {
		for (int i = 0; i < 16; i++) {
			if (targetTile == activeTilesPlayerTwo[i]) {
				activeTilesPlayerTwo[i] = std::pair<char,int>('0',0);
				resultState = ResponseState::HIT_ENEMY_SHIP;
				break;
			}
		}
		for (int i = 0; i < 16; i++) {
			if (activeTilesPlayerTwo[i].first != '0') {
				allZeros = false;
			}
		}
		if (allZeros) {
			resultState = ResponseState::WIN_PLAYER_ONE;
		}
	}

	return resultState;
}
