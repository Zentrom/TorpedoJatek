// GLEW
#include <GL/glew.h>



#include "TorpedoJatekClient.h"
#include "GameInstance.h"

TorpedoJatekClient::TorpedoJatekClient(void)
{
	
}

TorpedoJatekClient::~TorpedoJatekClient(void)
{
}

int TorpedoJatekClient::Start()
{
	// állítsuk be, hogy kilépés elõtt hívja meg a rendszer az exitProgram() függvényt
	atexit(exitProgram);

	if (this->Init()) {
		return 1;
	}

	if (this->CreateWindo()) {
		return 1;
	}

	if (this->StartGameInstance()) {
		return 1;
	}

	return 0;
}

int TorpedoJatekClient::Init()
{
	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezzük és lépjün ki
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		// irjuk ki a hibat es terminaljon a program
		std::cout << "[SDL indítása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
		return 1;
	}


	// beállíthatjuk azt, hogy pontosan milyen OpenGL context-et szeretnénk létrehozni - ha nem tesszük, akkor
	// automatikusan a legmagasabb elérhetõ verziójút kapjuk
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// állítsuk be, hogy hány biten szeretnénk tárolni a piros, zöld, kék és átlátszatlansági információkat pixelenként
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// duplapufferelés
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// mélységi puffer hány bites legyen
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// antialiasing - ha kell
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);
	return 0;
}

int TorpedoJatekClient::CreateWindo()
{
	
	window_title << "TorpedoJatek v" << majorVersion << "." << betaVersion << "." << alphaVersion << experimentalVersion;

	
	win = SDL_CreateWindow(window_title.str().c_str(),		// az ablak fejléce
		rightOffset, downOffset, widthWindow, heightWindow,flagsWindow);			// megjelenítési tulajdonságok


	// ha nem sikerült létrehozni az ablakot, akkor írjuk ki a hibát, amit kaptunk és lépjünk ki
	if (win == 0)
	{
		std::cout << "[Ablak létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
		return 1;
	}

	//
	// 3. lépés: hozzunk létre az OpenGL context-et - ezen keresztül fogunk rajzolni
	//

	context = SDL_GL_CreateContext(win);
	if (context == 0)
	{
		std::cout << "[OGL context létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
		return 1;
	}

	// megjelenítés: várjuk be a vsync-et
	SDL_GL_SetSwapInterval(enableVsync);

	// indítsuk el a GLEW-t
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::cout << "[GLEW] Hiba az inicializálás során!" << std::endl;
		return 1;
	}

	// kérdezzük le az OpenGL verziót
	int glVersion[2] = { -1, -1 };
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
	std::cout << "Running OpenGL " << glVersion[0] << "." << glVersion[1] << std::endl;

	if (glVersion[0] == -1 && glVersion[1] == -1)
	{
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(win);

		std::cout << "[OGL context létrehozása] Nem sikerült létrehozni az OpenGL context-et! Lehet, hogy az SDL_GL_SetAttribute(...) hívásoknál az egyik beállítás helytelen." << std::endl;

		return 1;
	}

	return 0;
}

int TorpedoJatekClient::StartGameInstance()
{
	bool quit = false;
	// feldolgozandó üzenet ide kerül
	SDL_Event ev;

	GameInstance gameInstance;

	if (!gameInstance.Init())
	{
		SDL_DestroyWindow(win);
		std::cout << "[app.Init] Az alkalmazás inicializálása közben hibatörtént!" << std::endl;
		return 1;
	}

	int frame_count = 0;
	int last_time = SDL_GetTicks();
	int last_render_time = SDL_GetTicks();
	int time_diff = 0;
	int ftime_diff = 0;

	//std::cout << 56 / 100 << std::endl;

	float frmtime = 1000.0f / fpsLimit;
	//std::cout << frmtime;
	float frmTemp = 0;
	float frmMod = 0;
	//std::vector<int> avgs;
	//avgs.clear();
	
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
				if (ev.key.keysym.sym == SDLK_ESCAPE)
					quit = true;
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
		
		gameInstance.Update();
		if (!enableVsync) {
			if (enableFpsLimitMode == 1) {
				ftime_diff = SDL_GetTicks() - last_render_time;
				frmTemp = ftime_diff;

				if (frmTemp + frmMod >= frmtime) {
					gameInstance.Render();
					SDL_GL_SwapWindow(win);
					++frame_count;
					last_render_time = SDL_GetTicks();
					frmMod = (frmTemp+frmMod) - frmtime;
					//std::cout << frmTemp << std::endl;
				}
			}
			else if(enableFpsLimitMode == 2){
				last_render_time = SDL_GetTicks();
				gameInstance.Render();
				SDL_GL_SwapWindow(win);
				++frame_count;
				ftime_diff = SDL_GetTicks() - last_render_time;
				if (ftime_diff < 1000 / fpsLimit) {

					//gameInstance.Render();
					//SDL_GL_SwapWindow(win);
					SDL_Delay((1000 / fpsLimit) - ftime_diff);
					//++frame_count;
					//last_render_time = SDL_GetTicks();
				}
				else {
					//std::cout << (1000.f/fpsLimit) <<" ftime:" << fmod(1.0f,0.0f) << std::endl;
					SDL_Delay((1000 / fpsLimit) - (ftime_diff % (1000 / fpsLimit)));
					//SDL_Delay(2.5);
					//SDL_Delay((1000.0f / fpsLimit) - fmod(ftime_diff,(1000.0f / fpsLimit)));
				}
				//else {
				//ftime_diff = 0;
				//last_render_time = SDL_GetTicks();
				//}
			}
			else {
				gameInstance.Render();
				SDL_GL_SwapWindow(win);
				++frame_count;
			}

		}

		else {
			gameInstance.Render();
			SDL_GL_SwapWindow(win);
			++frame_count;
		}

		time_diff = SDL_GetTicks() - last_time;
		//time_diff += SDL_GetTicks() - last_time;
		if (time_diff >= 1000)
		{
			window_title.str(std::string());
			//window_title << time_diff / (float)frame_count << " ms, " << frame_count << " FPS";
			window_title << "TorpedoJatek v" << majorVersion << "." << betaVersion << "." << alphaVersion << experimentalVersion
				<< " | FPS:" << frame_count;
			SDL_SetWindowTitle(win, window_title.str().c_str());

			//avgs.push_back(frame_count);

			last_time = SDL_GetTicks();
			time_diff = 0;
			frame_count = 0;
			
		}
		//SDL_GL_SwapWindow(win);
	}


	gameInstance.Clean();

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	return 0;
}

void TorpedoJatekClient::exitProgram()
{
	SDL_Quit();

	std::cout << "Press a key to exit..." << std::endl;
	std::cin.get();
}