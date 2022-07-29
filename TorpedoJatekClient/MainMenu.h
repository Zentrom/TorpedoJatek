#pragma once

#include "Utility/gShaderProgram.h"
#include "Utility/gVertexBuffer.h"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <iostream>

//J�t�kmenet oszt�lya
class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	bool Init();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent& key);
	void KeyboardUp(SDL_KeyboardEvent& key);
	void MouseMove(SDL_MouseMotionEvent& mouse);
	void MouseDown(SDL_MouseButtonEvent& mouse);
	void MouseUp(SDL_MouseButtonEvent& mouse);
	void MouseWheel(SDL_MouseWheelEvent& wheel);
	void Resize(int width, int height);
private:
	void CreateFrameBuffer(int width, int height);
	void HandleState();

	//GameState gameState; //jelenlegi j�t�k�llapot

	gVertexBuffer vb_fbo; //Custom Quad Framebufferbe rajzoland� n�gyzet adata
	gShaderProgram sh_default; //Alap shader+postprocess
	GLuint dirL_depthStencilBuffer; //Custom Framebuffer m�lys�g-stencil buffere
	GLuint dirL_colorBuffer; //Custom Framebuffer sz�nbuffere
	GLuint dirL_frameBuffer; //Custom Framebuffer azonos�t�
	Sint32 mouseX; //Eg�r relat�v X koord
	Sint32 mouseY; //Eg�r relat�v Y koord

	bool dirL_frameBufferCreated = false;

	int viewportWidth; //Ablak canvasz�nak sz�less�ge
	int viewportHeight; //Ablak canvasz�nak magass�ga

};
