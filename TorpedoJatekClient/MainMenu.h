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

//Menü fõosztálya
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
	enum MenuState { //Menü állapota
		CLICKING = 1,
		TYPING,
	};
	void CreateFrameBuffer(int width, int height);
	//void HandleState();

	MenuState menuState = MenuState::CLICKING; //jelenlegi menüállapot

	MenuStateHandler* initialState = new MenuStateHandler();
	MenuStateHandler* connectState = new MenuStateHandler();

	MenuStateHandler* pCurrentState = initialState;

	gVertexBuffer vb_fbo; //Custom Quad Framebufferbe rajzolandó négyzet adata
	gShaderProgram sh_default; //Alap shader+postprocess
	//GLuint dirL_depthStencilBuffer; //Custom Framebuffer mélység-stencil buffere
	GLuint dirL_colorBuffer; //Custom Framebuffer színbuffere
	GLuint dirL_frameBuffer; //Custom Framebuffer azonosító
	Sint32 mouseX; //Egér relatív X koord
	Sint32 mouseY; //Egér relatív Y koord
	gVertexBuffer vb_background; //Háttérnek a modell adat
	std::array<GLuint, 2> bgTextures = { //Háttér textúrák
		GLUtils::TextureFromFile("Resources/Textures/MenuBg/bg4.bmp"),
		GLUtils::TextureFromFile("Resources/Textures/MenuBg/bg5.bmp")
	};

	float* mousePointedData; //Egér melyik objektumra mutat 3D picking
	int typingInInput = 0; //Ebbe az indexû inputboxba írunk
	bool dirL_frameBufferCreated = false;

	std::string connectIP;
	std::string connectPort;
	bool connectSignal = false; //Connect gombot megnyomtuk-e

	int viewportWidth; //Ablak canvaszának szélessége
	int viewportHeight; //Ablak canvaszának magassága

};
