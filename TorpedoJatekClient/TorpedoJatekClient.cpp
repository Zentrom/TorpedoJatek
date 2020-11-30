// GLEW
#include <GL/glew.h>

#include <iostream>
#include <sstream>

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
	// �ll�tsuk be, hogy kil�p�s el�tt h�vja meg a rendszer az exitProgram() f�ggv�nyt
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
	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezz�k �s l�pj�n ki
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		// irjuk ki a hibat es terminaljon a program
		std::cout << "[SDL ind�t�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
		return 1;
	}


	// be�ll�thatjuk azt, hogy pontosan milyen OpenGL context-et szeretn�nk l�trehozni - ha nem tessz�k, akkor
	// automatikusan a legmagasabb el�rhet� verzi�j�t kapjuk
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

int TorpedoJatekClient::CreateWindo()
{
	std::stringstream window_title;
	window_title << "TorpedoJatek v" << majorVersion << "." << betaVersion << "." << alphaVersion << experimentalVersion;

	
	win = SDL_CreateWindow(window_title.str().c_str(),		// az ablak fejl�ce
		rightOffset, downOffset, widthWindow, heightWindow,flagsWindow);			// megjelen�t�si tulajdons�gok


	// ha nem siker�lt l�trehozni az ablakot, akkor �rjuk ki a hib�t, amit kaptunk �s l�pj�nk ki
	if (win == 0)
	{
		std::cout << "[Ablak l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
		return 1;
	}

	//
	// 3. l�p�s: hozzunk l�tre az OpenGL context-et - ezen kereszt�l fogunk rajzolni
	//

	context = SDL_GL_CreateContext(win);
	if (context == 0)
	{
		std::cout << "[OGL context l�trehoz�sa]Hiba az SDL inicializ�l�sa k�zben: " << SDL_GetError() << std::endl;
		return 1;
	}

	// megjelen�t�s: v�rjuk be a vsync-et
	SDL_GL_SetSwapInterval(enableVsync);

	// ind�tsuk el a GLEW-t
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

int TorpedoJatekClient::StartGameInstance()
{
	bool quit = false;
	// feldolgozand� �zenet ide ker�l
	SDL_Event ev;

	GameInstance gameInstance;

	if (!gameInstance.Init())
	{
		SDL_DestroyWindow(win);
		std::cout << "[app.Init] Az alkalmaz�s inicializ�l�sa k�zben hibat�rt�nt!" << std::endl;
		return 1;
	}

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
		gameInstance.Render();

		SDL_GL_SwapWindow(win);
	}


	gameInstance.Clean();

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	return 0;
}

void TorpedoJatekClient::exitProgram()
{
	SDL_Quit();

	std::cout << "Kil�p�shez nyomj meg egy billenty�t..." << std::endl;
	std::cin.get();
}