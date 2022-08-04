#pragma once

#include "Source/Frontend/Menu/MenuStateHandler.h"
#include "Utility/gShaderProgram.h"
#include "Utility/gVertexBuffer.h"
#include "Utility/GLUtils.h"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <array>
#include <iostream>

//Men� f�oszt�lya
class MainMenu
{
public:
	MainMenu(int viewport_w = 800, int viewport_h = 600);
	~MainMenu();

	bool Init();

	bool Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent& key);
	void KeyboardUp(SDL_KeyboardEvent& key);
	void MouseMove(SDL_MouseMotionEvent& mouse);
	bool MouseDown(SDL_MouseButtonEvent& mouse);
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
	void CreateFrameBuffer(int width, int height);
	//void HandleState();

	MenuState menuState = MenuState::CLICKING; //jelenlegi men��llapot

	MenuStateHandler* initialState = new MenuStateHandler();
	MenuStateHandler* connectState = new MenuStateHandler();

	MenuStateHandler* pCurrentState = initialState;

	gVertexBuffer vb_fbo; //Custom Quad Framebufferbe rajzoland� n�gyzet adata
	gShaderProgram sh_default; //Alap shader+postprocess
	//GLuint dirL_depthStencilBuffer; //Custom Framebuffer m�lys�g-stencil buffere
	GLuint dirL_colorBuffer; //Custom Framebuffer sz�nbuffere
	GLuint dirL_frameBuffer; //Custom Framebuffer azonos�t�
	Sint32 mouseX; //Eg�r relat�v X koord
	Sint32 mouseY; //Eg�r relat�v Y koord
	gVertexBuffer vb_background; //H�tt�rnek a modell adat
	std::array<GLuint, 2> bgTextures = { //H�tt�r text�r�k
		GLUtils::TextureFromFile("Resources/Textures/MenuBg/bg4.bmp"),
		GLUtils::TextureFromFile("Resources/Textures/MenuBg/bg5.bmp")
	};

	float* mousePointedData; //Eg�r melyik objektumra mutat 3D picking
	int typingInInput = 0; //Ebbe az index� inputboxba �runk
	bool dirL_frameBufferCreated = false;

	std::string connectIP;
	std::string connectPort;
	bool connectSignal = false; //Connect gombot megnyomtuk-e

	int viewportWidth; //Ablak canvasz�nak sz�less�ge
	int viewportHeight; //Ablak canvasz�nak magass�ga

};
