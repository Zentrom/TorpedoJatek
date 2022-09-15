#pragma once

#include "Source/Frontend/Game/Mountain.h"
#include "Source/Frontend/Game/Fleet.h"
#include "Source/Frontend/Game/Terrain.h"
#include "Source/Frontend/Game/Sea.h"
#include "Source/Frontend/Game/Skybox.h"
#include "Source/Frontend/Game/TextHandler.h"
#include "Source/BackEnd/GameLogic.h"
#include "Source/Backend/EventHandler.h"

#include "Utility/gCamera.h"
#include "Utility/gShaderProgram.h"
#include "Utility/gVertexBuffer.h"

#include <GL/glew.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <math.h>

//Játékmenet osztálya
class GameInstance
{
public:
	GameInstance(int viewport_w = 800, int viewport_h = 600);
	~GameInstance();

	bool Init(std::string ip, std::string port);

	void Update();
	void Render();

	bool KeyboardDown(SDL_KeyboardEvent& key);
	void KeyboardUp(SDL_KeyboardEvent& key);
	void MouseMove(SDL_MouseMotionEvent& mouse);
	void MouseDown(SDL_MouseButtonEvent& mouse);
	void MouseUp(SDL_MouseButtonEvent& mouse, SDL_Window* window);
	void MouseWheel(SDL_MouseWheelEvent& wheel);
	void Resize(int width, int height);
private:
	void CreateFrameBuffer(int width, int height);
	void HandleGameState();

	GameState gameState; //jelenlegi játékállapot
	Skybox* skybox = new Skybox(); //Pályát körülvevõ skybox
	Mountain* mountain = new Mountain(); //Hegy
	Terrain* terrain = nullptr; //Föld
	Fleet* playerFleet = new Fleet(true); //Játékos hajói
	Fleet* enemyFleet = new Fleet(false); //Ellenfél hajói
	Sea* sea = new Sea(); //Játék és tengermezõk
	GameLogic* gameLogic = nullptr; //Háttérlogika
	EventHandler* eventHandler = nullptr; //Eseménykezelés
	TextHandler* textHandler = nullptr; //Szövegkezelõ

	gVertexBuffer vb_fbo; //Custom Quad Framebufferbe rajzolandó négyzet adata
	gShaderProgram sh_default; //Alap shader+postprocess
	gShaderProgram sh_dirLight; //Ide rajzoljuk a 3D-s teret
	GLuint dirL_depthStencilBuffer; //Custom Framebuffer mélység-stencil buffere
	GLuint dirL_colorBuffer; //Custom Framebuffer színbuffere
	GLuint dirL_frameBuffer; //Custom Framebuffer azonosító
	Sint32 mouseX; //Egér relatív X koord
	Sint32 mouseY; //Egér relatív Y koord
	
	const float fieldOfView = 45.0f; //Kamera látószöge
	const float viewDistance = 150.0f * TorpedoGLOBAL::Scale; //Milyen távolságon belül rajzolunk ki dolgokat
	const float cameraRestraintXZ = 6.0f; //Földterület hányadáig mehet a lockolt kamera
	const float cameraRestraintY = 4.0f; //Hegymagasság hányszorosára mehet felfele a kamera

	bool showText = true; //Szöveget megjelenítjük-e
	bool isError = false; //van-e error a jelenlegi állapotban
	bool outputWritten = false; //kiirtuk-e már az üzenetet
	bool stopOutput = false; //Játék végén már ne írjunk ki
	bool dirL_frameBufferCreated = false;
	float* mousePointedData = nullptr; //Melyik játékmezõre mutat az egér
	bool shotReceived = false; //Küldött-e már lövést a szerver nekünk
	bool shotSent = false; //Küldtünk-e már lövést
	int shipSizeInput = 0; //Mekkora hajót készülünk lerakni
	int winnerPlayerNum = 0; //Nyertes játékos+boolként is mûködik ha senki nem nyert még
	Sint32 beforeRotationX = 0; //Forgatás elõtti X koord
	Sint32 beforeRotationY = 0; //Forgatás elõtti Y koord

	int viewportWidth; //Ablak canvaszának szélessége
	int viewportHeight; //Ablak canvaszának magassága
	gCamera* cam_mainCamera; //Kamera

};

