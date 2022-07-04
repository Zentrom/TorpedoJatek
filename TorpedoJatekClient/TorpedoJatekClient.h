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

//Kliens f�oszt�ly
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

	//enum class State {GLOBAL_GAME,GLOBAL_MENU}; //J�t�kmenetbe vagyunk vagy men�ben
	//State GLOBAL_STATE = State::GLOBAL_MENU; //Men�ben kezd�nk

	const TorpedoVersion* clientVersion; //Kliens verzi�sz�ma
	SDL_Window* gameWindow = 0; //Windows ablak
	SDL_GLContext glContext; //OpenGL be�ll�t�si �llapot - ez m�r void* t�pusu
	SDL_Event* sdlEvent;  //SDL esem�ny kezel�je
	GameInstance* gameInstance; //A t�nyleges torpedo j�t�kmenet
	
	const Uint32 flagsWindow = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE; //win ablakhoz tartoz� SDL flagek.
	const int rightOffset = 10; //Ablak Jobbra mozgaz�s
	const int downOffset = 30; //Ablak le mozgat�s
	const int widthWindow = 800; //ablak sz�less�g
	const int heightWindow = 600; //ablak magass�g
	const int enableVsync = 1; //F�gg�leges szinkroniz�ci� bekapcsol�sa. (-1 eset�n adaptive vsync)
	const int fpsLimit = 120; //Ha van FPSlimit,akkor mennyi legyen a maximum
	const int enableFpsLimit = 0; //Ha nincs Vsync,akkor legyen FPSlimit?

	std::stringstream window_title;	//Ablak sz�veg a c�msorban

};

