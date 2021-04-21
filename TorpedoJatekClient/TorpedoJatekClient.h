#pragma once

#include "GameInstance.h"
#include "../CommonSource/TorpedoVersion.hpp"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>
#include <sstream>

//Kliens fõosztály
class TorpedoJatekClient
{
public:
	TorpedoJatekClient(void);
	~TorpedoJatekClient(void);

	int Start();
	int Init();
	int CreateGameWindow();
	int StartGameInstance();

private:
	//enum class State {GLOBAL_GAME,GLOBAL_MENU}; //Játékmenetbe vagyunk vagy menüben
	//State GLOBAL_STATE = State::GLOBAL_MENU; //Menüben kezdünk

	SDL_Window *win = 0;	//Windows ablak
	const unsigned int rightOffset = 100;	//Jobbra mozgazás
	const unsigned int downOffset = 100;	//le mozgatás
	const unsigned int widthWindow = 800;	//ablak szélessés
	const unsigned int heightWindow = 600;	//magasság
	const Uint32 flagsWindow = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE; //win ablakhoz tartozó SDL flagek.
	std::stringstream window_title;	//Ablak szöveg a címsorban

	SDL_GLContext	context;	//OpenGL beállítási állapot

	const int enableVsync = 1;	//Függõleges szinkronizáció bekapcsolása.
	const int fpsLimit = 120;	//Ha van FPSlimit,akkor mennyi legyen a maximum
	const int enableFpsLimit = 0;	//Ha nincs Vsync,akkor legyen FPSlimit?

	const TorpedoVersion clientVersion; //Kliens verziószáma
};

