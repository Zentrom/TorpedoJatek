#pragma once

#include "Source/Frontend/Mountain.h"
#include "Source/Frontend/Fleet.h"
#include "Source/Frontend/Terrain.h"
#include "Source/Frontend/Sea.h"
#include "Source/Frontend/Skybox.h"
#include "Source/BackEnd/GameLogic.h"
#include "Source/Backend/EventHandler.h"

#include "Utility/gCamera.h"
#include "Utility/gShaderProgram.h"
#include "Utility/gVertexBuffer.h"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
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

	void Update();
	void Render();

	bool KeyboardDown(SDL_KeyboardEvent& key);
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

	GameState gameState; //jelenlegi játékállapot
	std::vector<void*> stateRelatedData; //játékállapothoz kapcsolódó adat
	bool isError; //van-e error a jelenlegi állapotban
	bool outputWritten; //kiirtuk-e már az üzenetet

	// framebuffer dolgok
	gVertexBuffer m_fbo_vbo;
	bool dirL_frameBufferCreated;
	GLuint dirL_depthStencilBuffer;
	GLuint dirL_colorBuffer;
	GLuint dirL_frameBuffer;

	Sint32 mouseX; //Egér relatív X koord
	Sint32 mouseY; //Egér relatív Y koord
	float* mousePointedData; //Melyik játékmezõre mutat az egér

	gCamera	cam_mainCamera; //Kamera
	gShaderProgram sh_default; //Alap shader+postprocess
	gShaderProgram sh_playtile; //PlayTile pre-postprocess shader
	gShaderProgram sh_dirLight; //Ide rajzoljuk a 3D-s teret
	gShaderProgram sh_skybox; //Skybox shadere

	Skybox skybox; //Pályát körülvevõ skybox
	Mountain mountain;	//Hegy
	Terrain terrain;	//Föld

	Fleet playerFleet;	//Játékos hajói
	Fleet enemyFleet;	//Ellenfél hajói
	Sea sea;	//Játék és tengermezõk

	GameLogic gameLogic;	//Háttérlogika
	EventHandler eventHandler; //Eseménykezelés

	int mapSize;	//Játékpálya mérete

	bool shotReceived = false; //Küldött-e már lövést a szerver nekünk
	int shipSizeInput = 0; //Mekkora hajót készülünk lerakni
	int winnerPlayerNum = 0; //Nyertes játékos+boolként is mûködik ha senki nem nyert még
};

