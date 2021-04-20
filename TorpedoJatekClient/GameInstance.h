#pragma once

#include "Source/Frontend/Mountain.h"
#include "Source/Frontend/Fleet.h"
#include "Source/Frontend/Terrain.h"
#include "Source/Frontend/Sea.h"
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
	GameInstance(void);
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
	void Resize(int _w, int _h);
protected:
	SDL_Thread* inputThread; //Thread az inputkezeléshez

	gCamera			cam_mainCamera; //Kamera
	gShaderProgram	sh_dirLight; //Alap shader
	gShaderProgram  sh_playtile; //Játékmezõk shadere

	Mountain mountain;	//Hegy
	Terrain terrain;	//Föld

	Fleet playerFleet;	//Játékos hajói
	Fleet enemyFleet;	//Ellenfél hajói
	Sea sea;	//Játék és tengermezõk

	GameLogic gameLogic;	//Háttérlogika

	int mapSize;	//Játékpálya mérete
	/*bool is_filled = true;
	bool use_texture = true;
	bool use_normal_map = false;
	bool use_light = false;

	bool is_ambient = true;
	bool is_diffuse = true;
	bool is_specular = true;*/
};

