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

//Kliens fõosztály
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

	//enum class State {GLOBAL_GAME,GLOBAL_MENU}; //Játékmenetbe vagyunk vagy menüben
	//State GLOBAL_STATE = State::GLOBAL_MENU; //Menüben kezdünk

	const TorpedoVersion* clientVersion; //Kliens verziószáma
	SDL_Window* gameWindow = 0; //Windows ablak
	Uint32 flagsWindow = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN; //win ablakhoz tartozó SDL flagek.
	SDL_GLContext glContext; //OpenGL beállítási állapot - ez már void* típusu
	SDL_Event* sdlEvent;  //SDL esemény kezelõje
	GameInstance* gameInstance = nullptr; //A tényleges torpedo játékmenet
	MainMenu* mainMenu = nullptr; //Fõmenü

	const int rightOffset = 10; //Ablak Jobbra mozgazás
	const int downOffset = 30; //Ablak le mozgatás
	const int widthWindow = 640; //ablak szélesség - DEFAULT érték
	const int heightWindow = 480; //ablak magasság - DEFAULT érték
	const int fullscreen = 0; //Teljes képernyõ - DEFAULT érték
	const int musicVolume = 128; //Zene hangerõ - DEFAULT érték
	const int sfxVolume = 128; //Hangok hangerõ - DEFAULT érték
	const int enableVsync = 1; //Függõleges szinkronizáció bekapcsolása. (-1 esetén adaptive vsync) - DEFAULT érték
	const int fpsLimit = 120; //Ha van FPSlimit,akkor mennyi legyen a maximum
	const int enableFpsLimit = 0; //Ha nincs Vsync,akkor legyen FPSlimit?
	const std::string pathToOptions = std::string("Resources/options.cfg"); //Path a beállításokat tartalmazó fájlhoz
	std::map <std::string, int> options;

	std::stringstream window_title;	//Ablak szöveg a címsorban
	std::string connectionIP;
	std::string connectionPort;
};

