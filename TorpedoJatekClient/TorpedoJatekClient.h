#pragma once

#include "GameInstance.h"
#include "../CommonSource/TorpedoVersion.hpp"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>
#include <sstream>

//Kliens f�oszt�ly
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
	//enum class State {GLOBAL_GAME,GLOBAL_MENU}; //J�t�kmenetbe vagyunk vagy men�ben
	//State GLOBAL_STATE = State::GLOBAL_MENU; //Men�ben kezd�nk

	SDL_Window *win = 0;	//Windows ablak
	const unsigned int rightOffset = 100;	//Jobbra mozgaz�s
	const unsigned int downOffset = 100;	//le mozgat�s
	const unsigned int widthWindow = 800;	//ablak sz�less�s
	const unsigned int heightWindow = 600;	//magass�g
	const Uint32 flagsWindow = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE; //win ablakhoz tartoz� SDL flagek.
	std::stringstream window_title;	//Ablak sz�veg a c�msorban

	SDL_GLContext	context;	//OpenGL be�ll�t�si �llapot

	const int enableVsync = 1;	//F�gg�leges szinkroniz�ci� bekapcsol�sa.
	const int fpsLimit = 120;	//Ha van FPSlimit,akkor mennyi legyen a maximum
	const int enableFpsLimit = 0;	//Ha nincs Vsync,akkor legyen FPSlimit?

	const TorpedoVersion clientVersion; //Kliens verzi�sz�ma
};

