#pragma once

#include "GameInstance.h"
#include "../CommonSource/TorpedoVersion.hpp"
#include "Source/Globals.hpp"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>

#include <iostream>
#include <sstream>

//Kliens fõosztály
class TorpedoJatekClient
{
public:
	TorpedoJatekClient();
	~TorpedoJatekClient();

	int Run();

private:
	int Init();
	int CreateGameWindow();
	int StartGameInstance();

	//enum class State {GLOBAL_GAME,GLOBAL_MENU}; //Játékmenetbe vagyunk vagy menüben
	//State GLOBAL_STATE = State::GLOBAL_MENU; //Menüben kezdünk

	const TorpedoVersion* clientVersion; //Kliens verziószáma
	SDL_Window* gameWindow = 0; //Windows ablak
	SDL_GLContext glContext; //OpenGL beállítási állapot - ez már void* típusu
	SDL_Event* sdlEvent;  //SDL esemény kezelõje
	GameInstance* gameInstance; //A tényleges torpedo játékmenet
	
	const Uint32 flagsWindow = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE; //win ablakhoz tartozó SDL flagek.
	const int rightOffset = 10; //Ablak Jobbra mozgazás
	const int downOffset = 30; //Ablak le mozgatás
	const int widthWindow = 800; //ablak szélesség
	const int heightWindow = 600; //ablak magasság
	const int enableVsync = 1; //Függõleges szinkronizáció bekapcsolása. (-1 esetén adaptive vsync)
	const int fpsLimit = 120; //Ha van FPSlimit,akkor mennyi legyen a maximum
	const int enableFpsLimit = 0; //Ha nincs Vsync,akkor legyen FPSlimit?

	std::stringstream window_title;	//Ablak szöveg a címsorban

};

