#pragma once

#include "Source/Frontend/Mountain.h"
#include "Source/Frontend/Fleet.h"
#include "Source/Frontend/Terrain.h"
#include "Source/Frontend/Sea.h"
#include "Source/Frontend/Skybox.h"
#include "Source/BackEnd/GameLogic.h"

#include "Utility/gCamera.h"
#include "Utility/gShaderProgram.h"
#include "Utility/gVertexBuffer.h"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_thread.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <math.h>

//J�t�kmenet oszt�lya
class GameInstance
{
public:
	GameInstance(float viewportW=800.0f, float viewportH=600.0f);
	~GameInstance(void);

	bool Init();
	void Clean();

	int static threadFunction(void* ptr);

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent& key);
	void KeyboardUp(SDL_KeyboardEvent& key);
	void MouseMove(SDL_MouseMotionEvent& mouse);
	void MouseDown(SDL_MouseButtonEvent& mouse);
	void MouseUp(SDL_MouseButtonEvent& mouse);
	void MouseWheel(SDL_MouseWheelEvent& wheel);
	void Resize(int w, int h);
protected:
	void CreateFrameBuffer(int width, int height);

	float viewportWidth; //Ablak canvasz�nak sz�less�ge
	float viewportHeight; //Ablak canvasz�nak magass�ga
	const float fieldOfView = 45.0f; //Kamera l�t�sz�ge
	const float viewDistance = 200.0f * TorpedoGLOBAL::Scale; //Milyen t�vols�gon bel�l rajzolunk ki dolgokat

	// framebuffer dolgok
	gVertexBuffer m_fbo_vbo;
	bool dirL_frameBufferCreated;
	GLuint dirL_depthBuffer;
	GLuint dirL_colorBuffer;
	GLuint dirL_frameBuffer;
	float* mousePointedData; //Melyik j�t�kmez�re mutat az eg�r

	SDL_Thread* inputThread; //Thread az inputkezel�shez

	gCamera	cam_mainCamera; //Kamera
	gShaderProgram  sh_default; //Alap shader+postprocess
	gShaderProgram	sh_dirLight; //Ide rajzoljuk a 3D-s teret
	gShaderProgram  sh_skybox; //Skybox shadere

	Skybox skybox; //P�ly�t k�r�lvev� skybox
	Mountain mountain;	//Hegy
	Terrain terrain;	//F�ld

	Fleet playerFleet;	//J�t�kos haj�i
	Fleet enemyFleet;	//Ellenf�l haj�i
	Sea sea;	//J�t�k �s tengermez�k

	GameLogic gameLogic;	//H�tt�rlogika

	int mapSize;	//J�t�kp�lya m�rete

};

