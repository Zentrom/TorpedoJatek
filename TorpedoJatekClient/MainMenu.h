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

//Men� f�oszt�lya
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
	enum MenuState { //Men� �llapota
		CLICKING = 1,
		TYPING,
	};
	void CreateElements();
	void CreateFrameBuffer(int width, int height);

	MenuState menuState = MenuState::CLICKING; //jelenlegi men��llapot

	//Men� melyik r�sz�ben vagyunk
	MenuStateHandler* initialState = new MenuStateHandler();
	MenuStateHandler* connectState = new MenuStateHandler();
	OptionHandler* optionsState = nullptr;

	MenuStateHandler* pCurrentState = initialState; //Jelenlegi men�r�sz

	gVertexBuffer vb_fbo; //Custom Quad Framebufferbe rajzoland� n�gyzet adata
	gShaderProgram sh_default; //Alap shader+postprocess
	//GLuint dirL_depthStencilBuffer; //Custom Framebuffer m�lys�g-stencil buffere
	GLuint dirL_colorBuffer; //Custom Framebuffer sz�nbuffere
	GLuint dirL_frameBuffer; //Custom Framebuffer azonos�t�
	Mix_Music* menuMusic = nullptr; //Men� zene adat
	Mix_Chunk* clickSound = nullptr; //Gombkattint�s hang adat
	Sint32 mouseX; //Eg�r relat�v X koord
	Sint32 mouseY; //Eg�r relat�v Y koord
	gVertexBuffer vb_background; //H�tt�rnek a modell adat
	std::array<GLuint, 4> bgTextures = { //H�tt�r text�r�k
		GLUtils::TextureFromFile("Resources/Textures/Menu/bg5.bmp"),
		GLUtils::TextureFromFile("Resources/Textures/Menu/bg4.bmp"),
		GLUtils::TextureFromFile("Resources/Textures/Menu/bg3.bmp"),
		GLUtils::TextureFromFile("Resources/Textures/Menu/bg2.bmp")
	};
	GLuint logoTexture = GLUtils::TextureFromFile("Resources/Textures/Menu/logo.bmp"); //J�t�k log� text�r�ja
	GLuint elementsBg = GLUtils::TextureFromFile("Resources/Textures/Menu/elementsBg.bmp"); //Men�elemeket k�rbefoglal� h�tt�r
	GLuint menuRail = GLUtils::TextureFromFile("Resources/Textures/Menu/rail.bmp");
	std::string versionString; //Verzi� sz�vegesen

	const float bgAnimTime = 4.0f; //H�tt�r anim�ci� ideje
	const float typingAnimTime = 0.5f; //Id� am�g �r�s k�zben a kurzor elt�nik/megjelenik

	int bgIndex = 0; //�ppen h�nyadik h�tteret mutatjuk
	float bgAnimElapsed = 0.0f; //Sz�ml�l� h�tt�ranimhoz
	bool cursorShown = true; //�ppen mutatjuk-e a kurzort inputboxba ir�skor
	float typingAnimElapsed = 0.0f; //Kurzoranimhoz a sz�ml�l�
	float* mousePointedData; //Eg�r melyik objektumra mutat 3D picking
	int typingInInput = 0; //Ebbe az index� inputboxba �runk
	bool dirL_frameBufferCreated = false;

	std::string connectIP; //IPhez gy�jt�
	std::string connectPort; //Porthoz gy�jt�

	int viewportWidth; //Ablak canvasz�nak sz�less�ge
	int viewportHeight; //Ablak canvasz�nak magass�ga
};
