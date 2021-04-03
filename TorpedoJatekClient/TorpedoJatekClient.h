#pragma once

#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>
#include <sstream>

#include "../CommonSource/TorpedoVersion.h"

class TorpedoJatekClient
{
public:
	TorpedoJatekClient(void);
	~TorpedoJatekClient(void);

	int Start();
	int Init();
	int CreateGameWindow();
	int StartGameInstance();

//protected:

private:
	enum class State {GLOBAL_GAME,GLOBAL_MENU};
	State GLOBAL_STATE = State::GLOBAL_MENU;

	//window
	SDL_Window *win = 0;
	const unsigned int rightOffset = 100;
	const unsigned int downOffset = 100;
	const unsigned int widthWindow = 800;
	const unsigned int heightWindow = 600;
	const Uint32 flagsWindow = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	std::stringstream window_title;

	SDL_GLContext	context;

	const int enableVsync = 1;
	const int fpsLimit = 120;
	const int enableFpsLimitMode = 0;

	//jatekverzio
	//const unsigned int majorVersion = 0;
	//const unsigned int betaVersion = 0;
	//const unsigned int alphaVersion = 1;
	//const char experimentalVersion = 'f';
	const TorpedoVersion clientVersion;
};

