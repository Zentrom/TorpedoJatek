
#include "TorpedoJatekClient.h"

TorpedoJatekClient::TorpedoJatekClient(void)
{
}

TorpedoJatekClient::~TorpedoJatekClient(void)
{
	SDL_Quit();
	std::cout << "Press enter to exit..." << std::endl;
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
}

//Elind�tja a klienst
int TorpedoJatekClient::Start()
{
	if (Init()) {
		return 1;
	}
	if (CreateGameWindow()) {
		return 1;
	}
	if (StartGameInstance()) {
		return 1;
	}
	return 0;
}

//Inicializ�lja a seg�dk�nyvt�rakat
int TorpedoJatekClient::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		std::cout << "[SDL ind�t�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
		return 1;
	}

	//explicit OPENGL verzio beallitas
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// �ll�tsuk be, hogy h�ny biten szeretn�nk t�rolni a piros, z�ld, k�k �s �tl�tszatlans�gi inform�ci�kat pixelenk�nt
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// duplapufferel�s
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// m�lys�gi puffer h�ny bites legyen
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// antialiasing - ha kell
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);
	return 0;
}

//SDL-el lek�r egy ablakot a Windowst�l
int TorpedoJatekClient::CreateGameWindow()
{

	window_title << "TorpedoJatek v" << clientVersion.majorVersion << "." << clientVersion.betaVersion
		<< "." << clientVersion.alphaVersion << clientVersion.experimentalVersion;

	win = SDL_CreateWindow(window_title.str().c_str(),
		rightOffset, downOffset, widthWindow, heightWindow, flagsWindow);

	if (win == 0)
	{
		std::cout << "[Ablak l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
		return 1;
	}

	context = SDL_GL_CreateContext(win);
	if (context == 0)
	{
		std::cout << "[OGL context l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
		return 1;
	}

	//VSync
	SDL_GL_SetSwapInterval(enableVsync);

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::cout << "[GLEW] Hiba az inicializ�l�s sor�n!" << std::endl;
		return 1;
	}

	// k�rdezz�k le az OpenGL verzi�t
	int glVersion[2] = { -1, -1 };
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
	std::cout << "Running OpenGL " << glVersion[0] << "." << glVersion[1] << std::endl;

	if (glVersion[0] == -1 && glVersion[1] == -1)
	{
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(win);
		std::cout << "[OGL context l�trehoz�sa] Nem siker�lt l�trehozni az OpenGL context-et! Lehet, hogy az SDL_GL_SetAttribute(...) h�v�sokn�l az egyik be�ll�t�s helytelen." << std::endl;
		return 1;
	}

	return 0;
}

//Elind�t egy j�t�kmenetet
int TorpedoJatekClient::StartGameInstance()
{
	bool quit = false;
	SDL_Event ev;

	GameInstance gameInstance(static_cast<float>(widthWindow),static_cast<float>(heightWindow));

	if (!gameInstance.Init())
	{
		SDL_DestroyWindow(win);
		std::cout << "[app.Init] Az alkalmaz�s inicializ�l�sa k�zben hibat�rt�nt!" << std::endl;
		return 1;
	}

	int frame_count = 0;
	int last_time = SDL_GetTicks();
	int last_render_time = SDL_GetTicks();
	int time_diff = 0;
	int ftime_diff = 0;

	float frmtime = 1000.0f / fpsLimit;
	float frmMod = 0;

	while (!quit)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (ev.key.keysym.sym == SDLK_ESCAPE) {
					//if (SDL_GetRelativeMouseMode() == SDL_bool(true)) {
					//	SDL_SetRelativeMouseMode(SDL_bool(false));
					//}
					//else {
						quit = true;
					//}
				}
				gameInstance.KeyboardDown(ev.key);
				break;
			case SDL_KEYUP:
				gameInstance.KeyboardUp(ev.key);
				break;
			case SDL_MOUSEBUTTONDOWN:
				gameInstance.MouseDown(ev.button);
				break;
			case SDL_MOUSEBUTTONUP:
				gameInstance.MouseUp(ev.button);
				break;
			case SDL_MOUSEWHEEL:
				gameInstance.MouseWheel(ev.wheel);
				break;
			case SDL_MOUSEMOTION:
				gameInstance.MouseMove(ev.motion);
				break;
			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					gameInstance.Resize(ev.window.data1, ev.window.data2);
				}
				break;
			}
		}

		//FPS limiter
		if (!enableVsync) {
			if (enableFpsLimit) {
				ftime_diff = SDL_GetTicks() - last_render_time;

				if (ftime_diff + frmMod >= frmtime) {
					gameInstance.Update();
					gameInstance.Render();
					SDL_GL_SwapWindow(win);
					++frame_count;
					last_render_time = SDL_GetTicks();
					frmMod = (ftime_diff + frmMod) - frmtime;
				}
			}
			else {
				gameInstance.Update();
				gameInstance.Render();
				SDL_GL_SwapWindow(win);
				++frame_count;
			}

		}

		else {
			gameInstance.Update();
			gameInstance.Render();
			SDL_GL_SwapWindow(win);
			++frame_count;
		}

		//FPS kiir�sa
		time_diff = SDL_GetTicks() - last_time;
		if (time_diff >= 1000)
		{
			window_title.str(std::string());
			window_title << "TorpedoJatek v" << clientVersion.majorVersion << "." << clientVersion.betaVersion
				<< "." << clientVersion.alphaVersion << clientVersion.experimentalVersion
				<< " | FPS:" << frame_count;
			SDL_SetWindowTitle(win, window_title.str().c_str());

			last_time = SDL_GetTicks();
			time_diff = 0;
			frame_count = 0;
		}
	}

	gameInstance.Clean();

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	return 0;
}
