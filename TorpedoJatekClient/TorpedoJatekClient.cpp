
#include "TorpedoJatekClient.h"

bool TorpedoGLOBAL::Debug = false;

TorpedoJatekClient::TorpedoJatekClient()
{
	clientVersion = new TorpedoVersion();
}

TorpedoJatekClient::~TorpedoJatekClient()
{
	Mix_CloseAudio();

	if (mainMenu) delete mainMenu;
	if (gameInstance) delete gameInstance;
	delete sdlEvent;
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(gameWindow);
	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();

	delete clientVersion;
}

//Elindítja a klienst
int TorpedoJatekClient::Run()
{
	if (Init()) {
		return 1;
	}
	if (ReadOptions()) {
		return 1;
	}
	if (CreateGameWindow()) {
		return 1;
	}
	bool quit = false;
	if (!StartMainMenu()) {
		if (StartGameInstance()) {
			return 1;
		}
	}
	return 0;
}

//Inicializálja a segédkönyvtárakat
int TorpedoJatekClient::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)
	{
		std::cout << "[SDL_Init]Error during SDL Video and Audio Init: " << SDL_GetError() << std::endl;
		return 1;
	}

	//hány biten szeretnénk tárolni a piros, zöld, kék és átlátszatlansági információkat pixelenként
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// mélységi/stencil
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	// duplapufferelés
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	// antialiasing
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

	//Mixer API inicializálása
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio error: %s\n", SDL_GetError());
		return 1;
	}

	//Hangformátum dll-ek inicializálása
	int audioFormats = MIX_INIT_MP3 | MIX_INIT_OGG;
	if ((Mix_Init(audioFormats) & audioFormats) != audioFormats) {
		std::cout << "[Mix_Init]Failed to init required Ogg and Mp3 support: " << SDL_GetError() << std::endl;
		//return 1;
	}

	//JPG és PNG formátumok támogatásának a betöltése
	int imageFormats = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(imageFormats) & imageFormats) != imageFormats) {
		std::cout << "[IMG_Init]Failed to init required JPG and PNG support: " << SDL_GetError() << std::endl;
		return 1;
	}

	if (TTF_Init() != 0) {
		std::cout << "[TTF_Init]Failed to init TrueType Font support: " << SDL_GetError() << std::endl;
		return 1;
	}

	options["ResolutionWidth"] = widthWindow;
	options["ResolutionHeight"] = heightWindow;
	options["Fullscreen"] = fullscreen;
	options["Vsync"] = enableVsync;
	options["MusicVolume"] = musicVolume;
	options["SfxVolume"] = sfxVolume;

	return 0;
}

//Beolvassa vagy létrehozza a beállításos fájlt
int TorpedoJatekClient::ReadOptions()
{
	std::fstream optionsFile(pathToOptions, std::ios::in);
	if (!optionsFile.is_open()) {
		//Hiányzó fájl
		std::cout << "[ReadOptions] Config file missing! Resetting defaults." << std::endl;
		optionsFile.clear();
		optionsFile.open(pathToOptions, std::ios::out);
		for (std::map<std::string, int>::const_iterator iter = options.cbegin(); iter != options.cend(); ++iter) {
			optionsFile << iter->first << '=' << iter->second << '\n';
		}
		optionsFile.close();
	}
	else {
		//Fájl szöveg ellenõrzése
		std::string line;
		int findIndex = 0;
		bool corrupted = false;
		for (std::map<std::string, int>::const_iterator iter = options.cbegin(); iter != options.cend(); ++iter) {
			std::getline(optionsFile, line);
			findIndex = line.find(iter->first + '=', 0);
			if (findIndex == std::string::npos) {
				corrupted = true;
				break;
			}
		}
		optionsFile.close();
		if (corrupted) {
			//Hibás fájl
			std::cout << "[ReadOptions] Config file corrupted! Resetting defaults." << std::endl;
			optionsFile.open(pathToOptions, std::ios::out|std::ios::trunc);
			for (std::map<std::string, int>::const_iterator iter = options.cbegin(); iter != options.cend(); ++iter) {
				optionsFile << iter->first << '=' << iter->second << '\n';
			}
			optionsFile.close();
		}
		else {
			//Jó fájl
			optionsFile.open(pathToOptions, std::ios::in);
			while (std::getline(optionsFile, line)) {
				findIndex = line.find('=', 0);
				std::map<std::string, int>::iterator it = options.find(line.substr(0, findIndex));

				if (it != options.end()) {
					it->second = std::stoi(line.substr(findIndex + 1));
				}

				//std::cout << line.substr(0, findIndex) << std::endl << "-------" << std::endl;
				//std::cout << line.substr(findIndex + 1) << std::endl;
			}
			optionsFile.close();

			if (!CheckOptionsIntegrity()) {
				//Rossz adatok a fájlban
				std::cout << "[CheckOptionsIntegrity] Config file corrupted! Fixing with defaults." << std::endl;
				optionsFile.open(pathToOptions, std::ios::out | std::ios::trunc);
				for (std::map<std::string, int>::const_iterator iter = options.cbegin(); iter != options.cend(); ++iter) {
					optionsFile << iter->first << '=' << iter->second << '\n';
				}
				optionsFile.close();
			}
		}
	}

	return 0;
}

//Leellenõrzi hogy a beállítások határon belül vannak-e
bool TorpedoJatekClient::CheckOptionsIntegrity()
{
	bool foundError = false;
	//if (options["ResolutionWidth"] < 0 || options["ResolutionWidth"]>1920) {
	//	options["ResolutionWidth"] = widthWindow;
	//	foundError = true;
	//}
	//if (options["ResolutionHeight"] < 0 || options["ResolutionHeight"]>1080) {
	//	options["ResolutionHeight"] = heightWindow;
	//	foundError = true;
	//}
	SDL_DisplayMode mode;
	bool foundResolution = false;
	for (int i = 0; i < SDL_GetNumDisplayModes(0); ++i) {
		if (SDL_GetDisplayMode(0, i, &mode) != 0) {
			std::cout << "[OptionHandler] SDL_GetDisplayMode failed: " << SDL_GetError() << std::endl;
		}
		else {
			if (mode.w == options["ResolutionWidth"] && mode.h == options["ResolutionHeight"]) {
				foundResolution = true;
				break;
			}
		}
	}
	if (!foundResolution) {
		options["ResolutionWidth"] = widthWindow;
		options["ResolutionHeight"] = heightWindow;
		foundError = true;
	}

	if (options["Fullscreen"] != 0 && options["Fullscreen"] != 1) {
		options["Fullscreen"] = fullscreen;
		foundError = true;
	}
	if (options["Vsync"] != 0 && options["Vsync"] != 1) {
		options["Vsync"] = enableVsync;
		foundError = true;
	}
	if (options["MusicVolume"] < 0 || options["MusicVolume"] > 128) {
		options["MusicVolume"] = musicVolume;
		foundError = true;
	}
	if (options["SfxVolume"] < 0 || options["SfxVolume"] > 128) {
		options["SfxVolume"] = sfxVolume;
		foundError = true;
	}
	if (foundError) return false;
	return true;
}

//SDL-el lekér egy ablakot a Windowstól
int TorpedoJatekClient::CreateGameWindow()
{
	window_title << "TorpedoJatek v" << clientVersion->majorVersion << "." << clientVersion->betaVersion
		<< "." << clientVersion->alphaVersion << clientVersion->experimentalVersion;

	if (options["Fullscreen"]) flagsWindow |= SDL_WINDOW_FULLSCREEN;
	gameWindow = SDL_CreateWindow(window_title.str().c_str(),
		rightOffset, downOffset, options["ResolutionWidth"], options["ResolutionHeight"], flagsWindow);

	if (gameWindow == nullptr)
	{
		std::cout << "[SDL_CreateWindow]Failed to create window: " << SDL_GetError() << std::endl;
		return 1;
	}

	glContext = SDL_GL_CreateContext(gameWindow);

	if (glContext == 0)
	{
		std::cout << "[SDL_GL_CreateContext]Failed to create GLcontext: " << SDL_GetError() << std::endl;
		return 1;
	}

	//VSync
	SDL_GL_SetSwapInterval(options["Vsync"]);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "[glewInit]Failed to init glew!" << std::endl;
		return 1;
	}

	//OpenGL verzió
	int glVersion[2] = { -1, -1 };
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
	std::cout << "Running OpenGL Version " << glVersion[0] << "." << glVersion[1] << std::endl;
	if (glVersion[0] == -1 && glVersion[1] == -1)
	{
		std::cout << "[OGLcontext]Error creating OpenGL context! One of the SDL_GL_SetAttribute(...) calls might be wrong." << std::endl;
		return 1;
	}
	sdlEvent = new SDL_Event();

	return 0;
}

//Menüt elindítja
int TorpedoJatekClient::StartMainMenu()
{
	mainMenu = new MainMenu(*clientVersion, options);
	if (!mainMenu->Init())
	{
		std::cout << "[mainMenu_Init] Main menu inicialization failed!" << std::endl;
		return 1;
	}

	int frame_count = 0;
	int last_time = SDL_GetTicks();
	int last_render_time = SDL_GetTicks();
	int time_diff = 0;
	int ftime_diff = 0;

	float frmtime = 1000.0f / fpsLimit;
	float frmMod = 0;

	//Fõ event ciklus
	bool quit = false;
	while (!quit)
	{
		//FPS limiter
		if (!options["Vsync"] && enableFpsLimit) {
			ftime_diff = SDL_GetTicks() - last_render_time;

			if (ftime_diff + frmMod >= frmtime) {
				mainMenu->Update();
				mainMenu->Render();
				SDL_GL_SwapWindow(gameWindow);
				++frame_count;
				last_render_time = SDL_GetTicks();
				frmMod = (ftime_diff + frmMod) - frmtime;
			}
		}
		else {
			mainMenu->Update();
			mainMenu->Render();
			SDL_GL_SwapWindow(gameWindow);
			++frame_count;
		}

		//FPS kiirása
		time_diff = SDL_GetTicks() - last_time;
		if (time_diff >= 1000)
		{
			window_title.str(std::string());
			window_title << "TorpedoJatek v" << clientVersion->majorVersion << "." << clientVersion->betaVersion
				<< "." << clientVersion->alphaVersion << clientVersion->experimentalVersion
				<< " | FPS:" << frame_count;
			SDL_SetWindowTitle(gameWindow, window_title.str().c_str());

			last_time = SDL_GetTicks();
			time_diff = 0;
			frame_count = 0;
		}

		while (SDL_PollEvent(sdlEvent))
		{
			switch (sdlEvent->type)
			{
			case SDL_QUIT:
				quit = true;
				return 1;
				break;
			case SDL_KEYDOWN:
				mainMenu->KeyboardDown(sdlEvent->key);
				break;
			case SDL_KEYUP:
				mainMenu->KeyboardUp(sdlEvent->key);
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (mainMenu->MouseDown(sdlEvent->button, gameWindow)) {
				case MenuSignal::CONTINUE:
					break;
				case MenuSignal::CONNECT:
					connectionIP = mainMenu->getIP();
					connectionPort = mainMenu->getPort();
					quit = true;
					break;
				case MenuSignal::DEBUG:
					TorpedoGLOBAL::Debug = true;
					quit = true;
					break;
				case MenuSignal::QUIT:
					quit = true;
					return 1;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				mainMenu->MouseUp(sdlEvent->button);
				break;
			case SDL_MOUSEWHEEL:
				mainMenu->MouseWheel(sdlEvent->wheel);
				break;
			case SDL_MOUSEMOTION:
				mainMenu->MouseMove(sdlEvent->motion);
				break;
			case SDL_WINDOWEVENT:
				if (sdlEvent->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					mainMenu->Resize(sdlEvent->window.data1, sdlEvent->window.data2);
				}
				break;
			}
		}

	}

	return 0;
}

//Elindít egy játékmenetet
int TorpedoJatekClient::StartGameInstance()
{
	gameInstance = new GameInstance(options["ResolutionWidth"], options["ResolutionHeight"]);
	if (!gameInstance->Init(connectionIP, connectionPort))
	{
		std::cout << "[GameInstance_Init] Game instance inicialization failed!" << std::endl;
		return 1;
	}

	int frame_count = 0;
	int last_time = SDL_GetTicks();
	int last_render_time = SDL_GetTicks();
	int time_diff = 0;
	int ftime_diff = 0;

	float frmtime = 1000.0f / fpsLimit;
	float frmMod = 0;

	//Fõ event ciklus
	bool quit = false;
	while (!quit)
	{
		//FPS limiter
		if (!options["Vsync"] && enableFpsLimit) {
				ftime_diff = SDL_GetTicks() - last_render_time;

				if (ftime_diff + frmMod >= frmtime) {
					gameInstance->Update();
					gameInstance->Render();
					SDL_GL_SwapWindow(gameWindow);
					++frame_count;
					last_render_time = SDL_GetTicks();
					frmMod = (ftime_diff + frmMod) - frmtime;
				}
		}
		else{
			gameInstance->Update();
			gameInstance->Render();
			SDL_GL_SwapWindow(gameWindow);
			++frame_count;
		}

		//FPS kiirása
		time_diff = SDL_GetTicks() - last_time;
		if (time_diff >= 1000)
		{
			window_title.str(std::string());
			window_title << "TorpedoJatek v" << clientVersion->majorVersion << "." << clientVersion->betaVersion
				<< "." << clientVersion->alphaVersion << clientVersion->experimentalVersion
				<< " | FPS:" << frame_count;
			SDL_SetWindowTitle(gameWindow, window_title.str().c_str());

			last_time = SDL_GetTicks();
			time_diff = 0;
			frame_count = 0;
		}

		while (SDL_PollEvent(sdlEvent))
		{
			switch (sdlEvent->type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (gameInstance->KeyboardDown(sdlEvent->key)) {
					quit = true;
				}
				break;
			case SDL_KEYUP:
				gameInstance->KeyboardUp(sdlEvent->key);
				break;
			case SDL_MOUSEBUTTONDOWN:
				gameInstance->MouseDown(sdlEvent->button);
				break;
			case SDL_MOUSEBUTTONUP:
				gameInstance->MouseUp(sdlEvent->button, gameWindow);
				break;
			case SDL_MOUSEWHEEL:
				gameInstance->MouseWheel(sdlEvent->wheel);
				break;
			case SDL_MOUSEMOTION:
				gameInstance->MouseMove(sdlEvent->motion);
				break;
			case SDL_WINDOWEVENT:
				if (sdlEvent->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					gameInstance->Resize(sdlEvent->window.data1, sdlEvent->window.data2);
				}
				break;
			}
		}

	}

	return 0;
}
