#pragma once

#include <SDL.h>
#include <SDL_opengl.h>

class TorpedoJatekClient
{
public:
	TorpedoJatekClient(void);
	~TorpedoJatekClient(void);

	
	void static exitProgram();
	int Start();
	int Init();
	int CreateWindo();
	int StartGameInstance();

//protected:

private:
	enum State {GLOBAL_GAME,GLOBAL_MENU};
	State GLOBAL_STATE = GLOBAL_MENU;

	//window
	SDL_Window *win = 0;
	const unsigned int rightOffset = 100;
	const unsigned int downOffset = 100;
	const unsigned int widthWindow = 800;
	const unsigned int heightWindow = 600;
	const Uint32 flagsWindow = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

	SDL_GLContext	context;

	const bool enableVsync = 1;

	//jatekverzio
	const unsigned int majorVersion = 0;
	const unsigned int betaVersion = 0;
	const unsigned int alphaVersion = 0;
	const char experimentalVersion = 'd';
};

