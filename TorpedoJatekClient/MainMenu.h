#pragma once

#include "Source/Frontend/Menu/OptionHandler.h"
#include "../CommonSource/TorpedoVersion.hpp"
#include "Utility/gShaderProgram.h"
#include "Utility/gVertexBuffer.h"
#include "Utility/GLUtils.h"
#include "Source/Globals.hpp"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_mixer.h>

#include <array>
#include <iostream>

//Menü fõosztálya
class MainMenu
{
public:
	MainMenu(const TorpedoVersion& version, std::map<std::string, int>& options);
	~MainMenu();

	bool Init();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent& key);
	void KeyboardUp(SDL_KeyboardEvent& key);
	void MouseMove(SDL_MouseMotionEvent& mouse);
	MenuSignal MouseDown(SDL_MouseButtonEvent& mouse, SDL_Window* window);
	void MouseUp(SDL_MouseButtonEvent& mouse);
	void MouseWheel(SDL_MouseWheelEvent& wheel);
	void Resize(int width, int height);

	std::string getIP();
	std::string getPort();
private:
	enum MenuState { //Menü állapota
		CLICKING = 1,
		TYPING,
	};
	void CreateElements();
	void CreateFrameBuffer(int width, int height);

	MenuState menuState = MenuState::CLICKING; //jelenlegi menüállapot

	//Menü melyik részében vagyunk
	MenuStateHandler* initialState = new MenuStateHandler();
	MenuStateHandler* connectState = new MenuStateHandler();
	OptionHandler* optionsState = nullptr;

	MenuStateHandler* pCurrentState = initialState; //Jelenlegi menürész

	gVertexBuffer vb_fbo; //Custom Quad Framebufferbe rajzolandó négyzet adata
	gShaderProgram sh_default; //Alap shader+postprocess
	//GLuint dirL_depthStencilBuffer; //Custom Framebuffer mélység-stencil buffere
	GLuint dirL_colorBuffer; //Custom Framebuffer színbuffere
	GLuint dirL_frameBuffer; //Custom Framebuffer azonosító
	Mix_Music* menuMusic = nullptr; //Menü zene adat
	Mix_Chunk* clickSound = nullptr; //Gombkattintás hang adat
	Sint32 mouseX; //Egér relatív X koord
	Sint32 mouseY; //Egér relatív Y koord
	gVertexBuffer vb_background; //Háttérnek a modell adat
	std::array<GLuint, 4> bgTextures = { //Háttér textúrák
		GLUtils::TextureFromFile("Resources/Textures/Menu/bg5.bmp"),
		GLUtils::TextureFromFile("Resources/Textures/Menu/bg4.bmp"),
		GLUtils::TextureFromFile("Resources/Textures/Menu/bg3.bmp"),
		GLUtils::TextureFromFile("Resources/Textures/Menu/bg2.bmp")
	};
	GLuint logoTexture = GLUtils::TextureFromFile("Resources/Textures/Menu/logo.bmp"); //Játék logó textúrája
	GLuint elementsBg = GLUtils::TextureFromFile("Resources/Textures/Menu/elementsBg.bmp"); //Menüelemeket körbefoglaló háttér
	GLuint menuRail = GLUtils::TextureFromFile("Resources/Textures/Menu/rail.bmp");
	std::string versionString; //Verzió szövegesen

	const float bgAnimTime = 4.0f; //Háttér animáció ideje
	const float typingAnimTime = 0.5f; //Idõ amég írás közben a kurzor eltünik/megjelenik

	int bgIndex = 0; //Éppen hányadik hátteret mutatjuk
	float bgAnimElapsed = 0.0f; //Számláló háttéranimhoz
	bool cursorShown = true; //Éppen mutatjuk-e a kurzort inputboxba iráskor
	float typingAnimElapsed = 0.0f; //Kurzoranimhoz a számláló
	float* mousePointedData; //Egér melyik objektumra mutat 3D picking
	int typingInInput = 0; //Ebbe az indexû inputboxba írunk
	bool dirL_frameBufferCreated = false;

	std::string connectIP; //IPhez gyûjtõ
	std::string connectPort; //Porthoz gyûjtõ

	int viewportWidth; //Ablak canvaszának szélessége
	int viewportHeight; //Ablak canvaszának magassága
};
