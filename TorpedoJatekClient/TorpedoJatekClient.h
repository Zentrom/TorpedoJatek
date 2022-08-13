#pragma once

#include "MainMenu.h"
#include "GameInstance.h"
#include "../CommonSource/TorpedoVersion.hpp"
#include "Source/Globals.hpp"

#include <GL/glew.h>
#include <SDL.h>
//#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

//Kliens f�oszt�ly
class TorpedoJatekClient
{
public:
	TorpedoJatekClient();
	~TorpedoJatekClient();

	int Run();

private:
	int Init();
	int ReadOptions();
	bool CheckOptionsIntegrity();
	int CreateGameWindow();
	int StartMainMenu();
	int StartGameInstance();

	//enum class State {GLOBAL_GAME,GLOBAL_MENU}; //J�t�kmenetbe vagyunk vagy men�ben
	//State GLOBAL_STATE = State::GLOBAL_MENU; //Men�ben kezd�nk

	const TorpedoVersion* clientVersion; //Kliens verzi�sz�ma
	SDL_Window* gameWindow = 0; //Windows ablak
	Uint32 flagsWindow = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN; //win ablakhoz tartoz� SDL flagek.
	SDL_GLContext glContext; //OpenGL be�ll�t�si �llapot - ez m�r void* t�pusu
	SDL_Event* sdlEvent;  //SDL esem�ny kezel�je
	GameInstance* gameInstance = nullptr; //A t�nyleges torpedo j�t�kmenet
	MainMenu* mainMenu = nullptr; //F�men�

	const int rightOffset = 10; //Ablak Jobbra mozgaz�s
	const int downOffset = 30; //Ablak le mozgat�s
	const int widthWindow = 640; //ablak sz�less�g - DEFAULT �rt�k
	const int heightWindow = 480; //ablak magass�g - DEFAULT �rt�k
	const int fullscreen = 0; //Teljes k�perny� - DEFAULT �rt�k
	const int musicVolume = 128; //Zene hanger� - DEFAULT �rt�k
	const int sfxVolume = 128; //Hangok hanger� - DEFAULT �rt�k
	const int enableVsync = 1; //F�gg�leges szinkroniz�ci� bekapcsol�sa. (-1 eset�n adaptive vsync) - DEFAULT �rt�k
	const int fpsLimit = 120; //Ha van FPSlimit,akkor mennyi legyen a maximum
	const int enableFpsLimit = 0; //Ha nincs Vsync,akkor legyen FPSlimit?
	const std::string pathToOptions = std::string("Resources/options.cfg"); //Path a be�ll�t�sokat tartalmaz� f�jlhoz
	std::map <std::string, int> options;

	std::stringstream window_title;	//Ablak sz�veg a c�msorban
	std::string connectionIP;
	std::string connectionPort;
};

