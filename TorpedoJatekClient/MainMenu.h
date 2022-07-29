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

//Játékmenet osztálya
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

	//GameState gameState; //jelenlegi játékállapot

	gVertexBuffer vb_fbo; //Custom Quad Framebufferbe rajzolandó négyzet adata
	gShaderProgram sh_default; //Alap shader+postprocess
	GLuint dirL_depthStencilBuffer; //Custom Framebuffer mélység-stencil buffere
	GLuint dirL_colorBuffer; //Custom Framebuffer színbuffere
	GLuint dirL_frameBuffer; //Custom Framebuffer azonosító
	Sint32 mouseX; //Egér relatív X koord
	Sint32 mouseY; //Egér relatív Y koord

	bool dirL_frameBufferCreated = false;

	int viewportWidth; //Ablak canvaszának szélessége
	int viewportHeight; //Ablak canvaszának magassága

};
