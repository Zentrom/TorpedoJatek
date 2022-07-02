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

//J�t�kmenet oszt�lya
class GameInstance
{
public:
	GameInstance(float viewport_w = 800.0f, float viewport_h = 600.0f);
	~GameInstance();

	bool Init();

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
	void HandleGameState();

	GameState gameState; //jelenlegi j�t�k�llapot
	Skybox* skybox = new Skybox(); //P�ly�t k�r�lvev� skybox
	Mountain* mountain = new Mountain(); //Hegy
	Terrain* terrain; //F�ld
	Fleet* playerFleet = new Fleet(true); //J�t�kos haj�i
	Fleet* enemyFleet = new Fleet(false); //Ellenf�l haj�i
	Sea* sea = new Sea(); //J�t�k �s tengermez�k
	GameLogic* gameLogic; //H�tt�rlogika
	EventHandler* eventHandler = new EventHandler(); //Esem�nykezel�s

	gVertexBuffer vb_fbo; //Custom Quad Framebufferbe rajzoland� n�gyzet adata
	gShaderProgram sh_default; //Alap shader+postprocess
	gShaderProgram sh_playtile; //PlayTile pre-postprocess shader
	gShaderProgram sh_dirLight; //Ide rajzoljuk a 3D-s teret
	GLuint dirL_depthStencilBuffer; //Custom Framebuffer m�lys�g-stencil buffere
	GLuint dirL_colorBuffer; //Custom Framebuffer sz�nbuffere
	GLuint dirL_frameBuffer; //Custom Framebuffer azonos�t�
	Sint32 mouseX; //Eg�r relat�v X koord
	Sint32 mouseY; //Eg�r relat�v Y koord
	
	const float fieldOfView = 45.0f; //Kamera l�t�sz�ge
	const float viewDistance = 200.0f * TorpedoGLOBAL::Scale; //Milyen t�vols�gon bel�l rajzolunk ki dolgokat
	const float cameraRestraintXZ = 4.0f; //F�ldter�let h�nyad�ig mehet a lockolt kamera
	const float cameraRestraintY = 4.0f; //Hegymagass�g h�nyszoros�ra mehet felfele a kamera

	bool isError = false; //van-e error a jelenlegi �llapotban
	bool outputWritten = false; //kiirtuk-e m�r az �zenetet
	bool dirL_frameBufferCreated = false;
	float* mousePointedData; //Melyik j�t�kmez�re mutat az eg�r
	//Mix_Chunk* cannonFireSound; //Haj� l�v�s hangadatra mutat� pointer
	bool shotReceived = false; //K�ld�tt-e m�r l�v�st a szerver nek�nk
	int shipSizeInput = 0; //Mekkora haj�t k�sz�l�nk lerakni
	int winnerPlayerNum = 0; //Nyertes j�t�kos+boolk�nt is m�k�dik ha senki nem nyert m�g

	float viewportWidth; //Ablak canvasz�nak sz�less�ge
	float viewportHeight; //Ablak canvasz�nak magass�ga
	gCamera* cam_mainCamera; //Kamera
};

