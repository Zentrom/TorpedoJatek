#include "TorpedoJatekServer.h"

TorpedoJatekServer::TorpedoJatekServer()
{
	firstClient.playerNum = 1;
	secondClient.playerNum = 2;
	temporaryClient.playerNum = 3;
	clients.at(0) = &firstClient;
	clients.at(1) = &secondClient;
	clients.at(2) = &temporaryClient;

	DisplaySettings();

	ServerHandler::Init_SDL();
	ServerHandler::Init_SDLNet();
}

TorpedoJatekServer::~TorpedoJatekServer()
{
	delete serverVersion;

	SDLNet_Quit();
	SDL_Quit();
}

//Beállítási szöveg frissítése
void TorpedoJatekServer::DisplaySettings() {
	currentSettings.str("");
	currentSettings << "Server version: " << serverVersion->majorVersion << '.' << serverVersion->betaVersion << '.'
		<< serverVersion->alphaVersion << serverVersion->experimentalVersion << '\n';
	currentSettings << "Current map-size: " << mapSize << '\n';
	currentSettings << "Port used: " << port << '\n';
}

//Elindítja a szervert
int TorpedoJatekServer::Run() {

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

		//Menü opció választás - Ha stringstream fail-el akkor 0 íródik be
		std::getline(std::cin, input);
		if(!(std::stringstream(input) >> intInput).fail()) {
			setupOption = static_cast<SetupOptions>(intInput);
		}
		else {
			setupOption = SetupOptions::DUMMY_OPTION;
		}
		//Pályaméret választási opció
		if (setupOption == SetupOptions::CHANGE_MAPSIZE) {
			int tmpMapSize = 0;
			do {
				std::cout << "Choose new map-size(5 OR 7 OR 9): ";
				std::getline(std::cin, input);
				std::stringstream(input) >> tmpMapSize;
			} while (tmpMapSize != 5 && tmpMapSize != 7 && tmpMapSize != 9);
			mapSize = tmpMapSize;
			DisplaySettings();
		}
		//Portszám választási opció
		if (setupOption == SetupOptions::CHANGE_PORT) {
			do {
				std::cout << "New port number: ";
				std::getline(std::cin, input);
			} while ((std::stringstream(input) >> port).fail());
			DisplaySettings();
		}
	}

	//Indul a szerver
	if (setupOption == SetupOptions::START_SERVER) {
		std::cout << "--------------------------------------------------------------" << std::endl;
		AllocateShips();
		Init();
		PrepareMatch();
		StartMatch();
	}

	return 0;
}

//Szerver létrehozása
void TorpedoJatekServer::Init()
{
	socketSet = ServerHandler::AllocSocketSet(maxSockets);

	ServerHandler::ResolveHost(&ip, nullptr, port);

	std::cout << "[INFO] Creating server..." << std::endl;
	server = ServerHandler::TCP_Open(&ip);

	ServerHandler::TCP_AddSocket(socketSet, server);
}

//Lefoglalja a hajóknak szánt memóriaterületet
void TorpedoJatekServer::AllocateShips()
{
	int activeTileCount = 0;
	std::array<int, 4> shipArray;

	switch (mapSize) {
	case 5:
		shipArray = ShipCount::Five;
		break;
	case 7:
		shipArray = ShipCount::Seven;
		break;
	case 9:
		shipArray = ShipCount::Nine;
		break;
	}

	for (unsigned int i = 0; i < shipArray.size(); ++i) {
		activeTileCount += (i + 1) * shipArray[i];
	}

	shipCount = 0;
	for (int &cou : shipArray) {
		shipCount += cou;
	}

	for(int i = 0;i < 2;++i){
		clients.at(i)->ships.resize(shipCount);
		clients.at(i)->unreadTiles = activeTileCount;
		//for (int j = 0; j < shipCounts.size(); ++j) {
		//	for (int k = 0; k < j; ++k) {
		//		clients.at(i)->ships.at(k).resize(j + 1);
		//	}
		//}
	}
	
}

//Várunk két kliensre, és elkérjük a mezõket amiken vannak hajóik
void TorpedoJatekServer::PrepareMatch()
{
	while (firstClient.state != ClientState::IN_MATCH_SHOOTER || secondClient.state != ClientState::IN_MATCH_TAKER) {
		std::cout << "[INFO] Waiting for network activity..." << std::endl;
		if (ServerHandler::CheckSocket(socketSet, static_cast<Uint32>(-1))) {
			//Ha senki se csatlakozott vagy a 3.
			if (ServerHandler::SocketReady(server)) {
				HandleClientState(*clients.at(getFirstNotConnectedIndex()));
			}
			//Elsõ
			else if (firstClient.state != ClientState::NOT_CONNECTED && ServerHandler::SocketReady(firstClient.socket)) {
				HandleClientState(*clients.at(0));
			}
			//Második
			else if (secondClient.state != ClientState::NOT_CONNECTED && ServerHandler::SocketReady(secondClient.socket)) {
				HandleClientState(*clients.at(1));
			}
		}
	}
	//Ha kész a hajólerakás
	int signalReady = 1;
	ServerHandler::SendBinary(firstClient.socket, &signalReady, sizeof(int));
	ServerHandler::SendBinary(secondClient.socket, &signalReady, sizeof(int));
	std::cout << "[INFO] Both players ready. Starting match!" << std::endl;
}

//Lekezeljük egy kliens állapotát
void TorpedoJatekServer::HandleClientState(Client& client)
{
	//Még nem csatlakozott
	if (client.state == ClientState::NOT_CONNECTED) {
		client.socket = ServerHandler::TCP_Accept(server);
		ServerHandler::TCP_AddSocket(socketSet, client.socket);
		//Version check
		if (!CheckClientVersion(client.socket)) {
			std::cout << "[WARNING] Version mismatch at connected Client! Closing connection." << std::endl;
			SDLNet_TCP_DelSocket(socketSet, client.socket);
			SDLNet_TCP_Close(client.socket);
			return;
		}

		client.name.str("");
		client.name << "Player" << client.playerNum;
		//Ha tele a szerver akkor most a 3mas számot átküldi kliensnek
		if (client.playerNum > 2) {
			ServerHandler::SendBinary(client.socket, &client.playerNum, sizeof(int));
			std::cout << "[WARNING] Server is full! Rejected a connection." << std::endl;

			SDLNet_TCP_DelSocket(socketSet, client.socket);
			SDLNet_TCP_Close(client.socket);
		}
		else
		{
			//client.activeTiles.resize(activeTileCount);
			//Átküldjük a játékos számot
			std::cout << "[INFO] " << client.name.str() << " connected." << '(' << ++connectedPlayers << "/2)" << std::endl;
			ServerHandler::SendBinary(client.socket, &client.playerNum, sizeof(int));
			//Átküldjük a mapsizet
			ServerHandler::SendBinary(client.socket, &mapSize, sizeof(int));
			client.state = ClientState::RECEIVING_SHIPS;
			std::cout << "[INFO] Map size sent to " << client.name.str() << std::endl;
			
		}
	}
	//Ha hajókat várunk tõle
	else if (client.state == ClientState::RECEIVING_SHIPS) {
		receivedType = ServerHandler::ReceiveMessageType(client.socket);
		//Ha elvárt adat jött, a hajó pozíciók
		if (receivedType == MessageType::ESTIMATED) {
			//for (int i = 0; i < activeTileCount; ++i) {
			//	ServerHandler::ReceiveBinary(client.socket, &client.activeTiles.at(i), sizeof(std::pair<char, int>));
			//}
			int sentSize = 0;
			std::pair<char, int> data;
			while (client.unreadTiles > 0) {
				ServerHandler::ReceiveBinary(client.socket, &data, sizeof(std::pair<char, int>));
				if (data.first != 'v') {
					std::cout << "[WARNING] Corrupted ship-size message received from: " << client.name.str() << std::endl;
				}
				else {
					sentSize = data.second;
				}
				client.ships.push_back(std::vector<std::pair<char, int>>());
				for (int i = 0; i < sentSize; ++i) {
					ServerHandler::ReceiveBinary(client.socket, &data, sizeof(std::pair<char, int>));
					client.ships.back().push_back(data);
					--client.unreadTiles;
				}
			}

			std::cout << "[INFO] Received ShipData from " << client.name.str() << std::endl;
			if (client.playerNum == 1) {
				client.state = ClientState::IN_MATCH_SHOOTER;
			}
			else if (client.playerNum == 2) {
				client.state = ClientState::IN_MATCH_TAKER;
			}
		}
		//Ha kilépési szándék jött
		else if (receivedType == MessageType::QUIT) {
			std::cout << "[WARNING] " << client.name.str() << " has left the server! Closing connection." << std::endl;
			SDLNet_TCP_DelSocket(socketSet, client.socket);
			SDLNet_TCP_Close(client.socket);
			client.state = ClientState::NOT_CONNECTED;
			--connectedPlayers;
		}
	}
	//Kilépési szándék már a játék közben
	else if (client.state == ClientState::IN_MATCH_SHOOTER || client.state == ClientState::IN_MATCH_TAKER) {
		receivedType = ServerHandler::ReceiveMessageType(client.socket);
		if (receivedType == MessageType::QUIT) {
			std::cout << "[WARNING] " << client.name.str() << " has left the server! Closing connection." << std::endl;
			SDLNet_TCP_DelSocket(socketSet, client.socket);
			SDLNet_TCP_Close(client.socket);
			client.state = ClientState::NOT_CONNECTED;
			--connectedPlayers;
		}
	}
	//Le nem kezelt lehetõség
	else {
		std::cout << "[WARNING] Unwanted socket activity detected!(maybe client aborted): "
			<< client.name.str() << ". Closing connection." << std::endl;
		SDLNet_TCP_DelSocket(socketSet, firstClient.socket);
		SDLNet_TCP_Close(firstClient.socket);
		firstClient.state = ClientState::NOT_CONNECTED;
		--connectedPlayers;
	}
}

//Megnézi,hogy a csatlakozott kliens verziója megegyezik-e a szerverével
bool TorpedoJatekServer::CheckClientVersion(TCPsocket &connected_socket) {

	const char *text;
	int textLength;
	bool versionCheckSuccess;

	TorpedoVersion clientVersion;
	ServerHandler::ReceiveBinary(connected_socket, &clientVersion, sizeof(TorpedoVersion));

	if (serverVersion->majorVersion == clientVersion.majorVersion && serverVersion->betaVersion == clientVersion.betaVersion
		&& serverVersion->alphaVersion == clientVersion.alphaVersion
		&& serverVersion->experimentalVersion == clientVersion.experimentalVersion) {
		//Ha sikeres versioncheck
		std::cout << "[INFO] Connected socket passed the version check." << std::endl;
		versionCheckSuccess = true;
		text = "Version check passed.";
		textLength = strlen(text) + 1;
		ServerHandler::SendBinary(connected_socket, &versionCheckSuccess, sizeof(bool));
		ServerHandler::SendBinary(connected_socket, &textLength, sizeof(int));
		ServerHandler::SendBinary(connected_socket, text, strlen(text) + 1); //char=1byte,végére \0
		return true;
	}
	else {//Ha sikertelen versioncheck
		versionCheckSuccess = false;
		std::stringstream response;
		response << "Version check failed!\n" << "Server version is: v" << serverVersion->majorVersion << '.'
			<< serverVersion->betaVersion << '.' << serverVersion->alphaVersion << serverVersion->experimentalVersion;
		//EZ A TEMP KELL,stringstream.str() törli magát .c_str() hívás elõtt
		const std::string& tempStr = response.str();
		text = tempStr.c_str();
		textLength = strlen(text) + 1;
		ServerHandler::SendBinary(connected_socket, &versionCheckSuccess, sizeof(bool));
		ServerHandler::SendBinary(connected_socket, &textLength, sizeof(int));
		ServerHandler::SendBinary(connected_socket, text, strlen(text) + 1);
		return false;
	}
}

//Játékmeccs indítása
void TorpedoJatekServer::StartMatch() {
	//Amég valaki nem nyert
	while (firstClient.socket && secondClient.socket && responseState != ResponseState::WIN_PLAYER_ONE
		&& responseState != ResponseState::WIN_PLAYER_TWO) {

		if (ServerHandler::CheckSocket(socketSet, static_cast<Uint32>(-1))) {
			//Ha 3. ember csatlakozna már tele vagyunk
			if (ServerHandler::SocketReady(server)) {
				HandleClientState(temporaryClient);
			}
			//Elsõ játékos
			else if (ServerHandler::SocketReady(firstClient.socket)) {
				HandleShot(firstClient, secondClient);
			}
			//Második játékos
			else if (ServerHandler::SocketReady(secondClient.socket)) {
				HandleShot(secondClient, firstClient);
			}
		}
	}
	//Ha valaki nyert
	if (responseState == ResponseState::WIN_PLAYER_ONE) {
		std::cout << "[INFO] " << firstClient.name.str() << " won the match!" << std::endl;
		SDLNet_TCP_DelSocket(socketSet, firstClient.socket);

		if (!isDisconnection) {
			while (true) {
				if (ServerHandler::CheckSocket(socketSet, static_cast<Uint32>(-1))) {
					if (ServerHandler::SocketReady(secondClient.socket)) {
						int tmp;
						ServerHandler::ReceiveBinary(secondClient.socket, &tmp, sizeof(int));
						break;
					}
				}
			}
			SendShipsToLoser(secondClient);
		}

		SDLNet_TCP_DelSocket(socketSet, secondClient.socket);
	}
	else if (responseState == ResponseState::WIN_PLAYER_TWO) {
		std::cout << "[INFO] " << secondClient.name.str() << " won the match!" << std::endl;
		SDLNet_TCP_DelSocket(socketSet, secondClient.socket);

		if (!isDisconnection) {
			while (true) {
				if (ServerHandler::CheckSocket(socketSet, static_cast<Uint32>(-1))) {
					if (ServerHandler::SocketReady(firstClient.socket)) {
						int tmp;
						ServerHandler::ReceiveBinary(firstClient.socket, &tmp, sizeof(int));
						break;
					}
				}
			}
			SendShipsToLoser(firstClient);
		}

		SDLNet_TCP_DelSocket(socketSet, firstClient.socket);
	}

	std::cout << "[INFO] Game ended. Closing connections..." << std::endl;
	SDLNet_TCP_DelSocket(socketSet, server);
	SDLNet_FreeSocketSet(socketSet);

	SDLNet_TCP_Close(firstClient.socket);
	SDLNet_TCP_Close(secondClient.socket);
	SDLNet_TCP_Close(server);

	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << "Press enter to exit..." << std::endl;
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
}

//Egy lövés lekezelése
void TorpedoJatekServer::HandleShot(Client& shooter, Client& taker)
{
	receivedType = ServerHandler::ReceiveMessageType(shooter.socket);
	//Elvárt adat jött, lövési pos
	if (receivedType == MessageType::ESTIMATED) {
		ServerHandler::ReceiveBinary(shooter.socket, &targetTile, sizeof(std::pair<char, int>));

		responseState = ProcessTiles(taker);
		std::cout << "[INFO] " << shooter.name.str() << " shot at " << targetTile.first 
			<< targetTile.second << " -> " 
			<< (responseState==ResponseState::CONTINUE_MATCH ? "Miss" : "Hit") << std::endl;

		ServerHandler::SendBinary(shooter.socket, &responseState, sizeof(ResponseState));
		ServerHandler::SendBinary(taker.socket, &targetTile, sizeof(std::pair<char, int>));
		ServerHandler::SendBinary(taker.socket, &responseState, sizeof(ResponseState));

		shooter.state = ClientState::IN_MATCH_TAKER;
		taker.state = ClientState::IN_MATCH_SHOOTER;
	}
	//Kilépési szándék jött
	else if (receivedType == MessageType::QUIT) {
		
		isDisconnection = true;
		if (shooter.playerNum == 1) {
			responseState = ResponseState::WIN_PLAYER_TWO;
		}
		else if (shooter.playerNum == 2) {
			responseState = ResponseState::WIN_PLAYER_ONE;
		}
		if (shooter.state == ClientState::IN_MATCH_SHOOTER) {
			std::cout << "[WARNING] " << shooter.name.str() << " left the game! Ending game!" << std::endl;
			std::pair<char, int> dummyPair = std::pair<char, int>('0', 0);
			ServerHandler::SendBinary(taker.socket, &dummyPair, sizeof(std::pair<char, int>));
			ServerHandler::SendBinary(taker.socket, &responseState, sizeof(ResponseState));
		}
		//Ez lekezeli ha kilép az aki nem lõ éppen
		else {
			std::cout << "[WARNING] " << taker.name.str() << " left the game! Ending game!" << std::endl;
			receivedType = ServerHandler::ReceiveMessageType(taker.socket);
			ServerHandler::ReceiveBinary(taker.socket, &targetTile, sizeof(std::pair<char, int>));
			ServerHandler::SendBinary(taker.socket, &responseState, sizeof(ResponseState));
		}
	}
}

//A célzott játékmezõ lekezelése,és egy válaszállapot visszaadása
ResponseState TorpedoJatekServer::ProcessTiles(Client &client)
{
	ResponseState resultState = ResponseState::CONTINUE_MATCH;

	//ITT A RANGED-BASED LOOP NEM MEGY,NE PRÓBÁLD.NEMTOM MIÉRT.
	//for (int i = 0; i < activeTileCount; ++i) {
	//	if (targetTile == client.activeTiles[i]) {
	//		client.activeTiles[i] = std::pair<char, int>('0', 0);
	//		resultState = ResponseState::HIT_ENEMY_SHIP;
	//		break;
	//	}
	//}

	bool found = false;
	for (int i = 0; i < client.ships.size(); ++i) {
		for (int j = 0; j < client.ships.at(i).size(); ++j) {
			if (targetTile == client.ships.at(i).at(j)) {
				client.ships.at(i).at(j) = std::pair<char, int>('0', 0);
				resultState = ResponseState::HIT_ENEMY_SHIP;
				found = true;
				break;
			}
		}
		if (found) break;
	}

	bool allZeros = true;
	//for (int i = 0; i < activeTileCount; ++i) {
	//	if (client.activeTiles[i].first != '0') {
	//		allZeros = false;
	//		break;
	//	}
	//}
	for (int i = 0; i < client.ships.size(); ++i) {
		for (int j = 0; j < client.ships.at(i).size(); ++j) {
			if (client.ships.at(i).at(j).first != '0') {
				allZeros = false;
				break;
			}
		}
		if (!allZeros) break;
	}

	if (allZeros) {
		if (client.playerNum == 1) {
			resultState = ResponseState::WIN_PLAYER_TWO;
		}
		else if (client.playerNum == 2) {
			resultState = ResponseState::WIN_PLAYER_ONE;
		}
	}

	return resultState;
}

//Vesztes játékosnak elküldi a nyertes élõ hajóit
void TorpedoJatekServer::SendShipsToLoser(Client& client)
{
	Client* pWinner;
	if (client.playerNum == secondClient.playerNum) {
		pWinner = &firstClient;
	}
	else {
		pWinner = &secondClient;
	}

	int sentSize = 0;
	std::pair<char, int> data;
	for (int i = 0; i < pWinner->ships.size(); ++i) {
		
		bool isAlive = false;
		int aliveIndex = 0;
		for (int j = 0; j < pWinner->ships.at(i).size(); ++j) {
			if (pWinner->ships.at(i).at(j).first != '0') {
				isAlive = true;
				aliveIndex = i;
				break;
			}
		}

		if (isAlive) {
			sentSize = pWinner->ships.at(i).size();
			data.first = 'v';
			data.second = sentSize;
			ServerHandler::SendBinary(client.socket, &data, sizeof(std::pair<char, int>));
			std::cout << data.first << data.second << std::endl;
			for (int j = 0; j < pWinner->ships.at(aliveIndex).size(); ++j) {

				data.first = pWinner->ships.at(aliveIndex).at(j).first;
				data.second = pWinner->ships.at(aliveIndex).at(j).second;
				ServerHandler::SendBinary(client.socket, &data, sizeof(std::pair<char, int>));
				std::cout << data.first << data.second << std::endl;
			}
		}
	}
	data.first = 'x';
	data.second = 0;
	ServerHandler::SendBinary(client.socket, &data, sizeof(std::pair<char, int>));
	std::cout << data.first << data.second << std::endl;

	std::cout << "[INFO] Winner's alive ships sent to " << client.name.str() << std::endl;
}

//Visszaadja az elsõ nem csatlakozott kliens indexet
int TorpedoJatekServer::getFirstNotConnectedIndex() const
{
	for (unsigned int i = 0; i < clients.size(); ++i) {
		if (clients.at(i)->state == ClientState::NOT_CONNECTED) {
			return i;
		}
	}
	return 0;
}