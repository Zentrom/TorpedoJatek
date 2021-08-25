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

//Játékmenet osztálya
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

	float viewportWidth; //Ablak canvaszának szélessége
	float viewportHeight; //Ablak canvaszának magassága
	const float fieldOfView = 45.0f; //Kamera látószöge
	const float viewDistance = 200.0f * TorpedoGLOBAL::Scale; //Milyen távolságon belül rajzolunk ki dolgokat

	// framebuffer dolgok
	gVertexBuffer m_fbo_vbo;
	bool dirL_frameBufferCreated;
	GLuint dirL_depthBuffer;
	GLuint dirL_colorBuffer;
	GLuint dirL_frameBuffer;
	float* mousePointedData; //Melyik játékmezõre mutat az egér

	SDL_Thread* inputThread; //Thread az inputkezeléshez

	gCamera	cam_mainCamera; //Kamera
	gShaderProgram  sh_default; //Alap shader+postprocess
	gShaderProgram	sh_dirLight; //Ide rajzoljuk a 3D-s teret
	gShaderProgram  sh_skybox; //Skybox shadere

	Skybox skybox; //Pályát körülvevõ skybox
	Mountain mountain;	//Hegy
	Terrain terrain;	//Föld

	Fleet playerFleet;	//Játékos hajói
	Fleet enemyFleet;	//Ellenfél hajói
	Sea sea;	//Játék és tengermezõk

	GameLogic gameLogic;	//Háttérlogika

	int mapSize;	//Játékpálya mérete

};

