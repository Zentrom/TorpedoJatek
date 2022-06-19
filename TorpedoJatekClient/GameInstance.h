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
#include <SDL_mixer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <math.h>

//Játékmenet osztálya
class GameInstance
{
public:
	GameInstance(float viewport_w = 800.0f, float viewport_h = 600.0f);
	~GameInstance();

	bool Init();
	//void Clean();

	void Update();
	void Render();

	bool KeyboardDown(SDL_KeyboardEvent& key);
	void KeyboardUp(SDL_KeyboardEvent& key);
	void MouseMove(SDL_MouseMotionEvent& mouse);
	void MouseDown(SDL_MouseButtonEvent& mouse);
	void MouseUp(SDL_MouseButtonEvent& mouse);
	void MouseWheel(SDL_MouseWheelEvent& wheel);
	void Resize(int width, int height);
private:
	void CreateFrameBuffer(int width, int height);

	float viewportWidth; //Ablak canvaszának szélessége
	float viewportHeight; //Ablak canvaszának magassága
	const float fieldOfView = 45.0f; //Kamera látószöge
	const float viewDistance = 200.0f * TorpedoGLOBAL::Scale; //Milyen távolságon belül rajzolunk ki dolgokat

	GameState gameState; //jelenlegi játékállapot
	//std::vector<void*> stateRelatedData; //játékállapothoz kapcsolódó adat
	bool isError = false; //van-e error a jelenlegi állapotban
	bool outputWritten = false; //kiirtuk-e már az üzenetet

	//Framebuffer dolgok
	gVertexBuffer vb_fbo;
	bool dirL_frameBufferCreated = false;
	GLuint dirL_depthStencilBuffer;
	GLuint dirL_colorBuffer;
	GLuint dirL_frameBuffer;

	Sint32 mouseX; //Egér relatív X koord
	Sint32 mouseY; //Egér relatív Y koord
	float* mousePointedData; //Melyik játékmezõre mutat az egér

	gCamera* cam_mainCamera = new gCamera(glm::vec3(0, 20.0f, 20.0f)); //Kamera
	gShaderProgram sh_default; //Alap shader+postprocess
	gShaderProgram sh_playtile; //PlayTile pre-postprocess shader
	gShaderProgram sh_dirLight; //Ide rajzoljuk a 3D-s teret

	//Mix_Chunk* cannonFireSound; //Hajó lövés hangadatra mutató pointer

	Skybox* skybox = new Skybox(); //Pályát körülvevõ skybox
	Mountain* mountain = new Mountain(); //Hegy
	Terrain* terrain = new Terrain(); //Föld

	Fleet* playerFleet = new Fleet(); //Játékos hajói
	Fleet* enemyFleet = new Fleet(); //Ellenfél hajói
	Sea* sea = new Sea(); //Játék és tengermezõk

	GameLogic* gameLogic = new GameLogic(); //Háttérlogika
	EventHandler* eventHandler = new EventHandler(); //Eseménykezelés

	int mapSize;	//Játékpálya mérete

	bool shotReceived = false; //Küldött-e már lövést a szerver nekünk
	int shipSizeInput = 0; //Mekkora hajót készülünk lerakni
	int winnerPlayerNum = 0; //Nyertes játékos+boolként is mûködik ha senki nem nyert még
};

