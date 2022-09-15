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

//J�t�kmenet oszt�lya
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

	GameState gameState; //jelenlegi j�t�k�llapot
	Skybox* skybox = new Skybox(); //P�ly�t k�r�lvev� skybox
	Mountain* mountain = new Mountain(); //Hegy
	Terrain* terrain = nullptr; //F�ld
	Fleet* playerFleet = new Fleet(true); //J�t�kos haj�i
	Fleet* enemyFleet = new Fleet(false); //Ellenf�l haj�i
	Sea* sea = new Sea(); //J�t�k �s tengermez�k
	GameLogic* gameLogic = nullptr; //H�tt�rlogika
	EventHandler* eventHandler = nullptr; //Esem�nykezel�s
	TextHandler* textHandler = nullptr; //Sz�vegkezel�

	gVertexBuffer vb_fbo; //Custom Quad Framebufferbe rajzoland� n�gyzet adata
	gShaderProgram sh_default; //Alap shader+postprocess
	gShaderProgram sh_dirLight; //Ide rajzoljuk a 3D-s teret
	GLuint dirL_depthStencilBuffer; //Custom Framebuffer m�lys�g-stencil buffere
	GLuint dirL_colorBuffer; //Custom Framebuffer sz�nbuffere
	GLuint dirL_frameBuffer; //Custom Framebuffer azonos�t�
	Sint32 mouseX; //Eg�r relat�v X koord
	Sint32 mouseY; //Eg�r relat�v Y koord
	
	const float fieldOfView = 45.0f; //Kamera l�t�sz�ge
	const float viewDistance = 150.0f * TorpedoGLOBAL::Scale; //Milyen t�vols�gon bel�l rajzolunk ki dolgokat
	const float cameraRestraintXZ = 6.0f; //F�ldter�let h�nyad�ig mehet a lockolt kamera
	const float cameraRestraintY = 4.0f; //Hegymagass�g h�nyszoros�ra mehet felfele a kamera

	bool showText = true; //Sz�veget megjelen�tj�k-e
	bool isError = false; //van-e error a jelenlegi �llapotban
	bool outputWritten = false; //kiirtuk-e m�r az �zenetet
	bool stopOutput = false; //J�t�k v�g�n m�r ne �rjunk ki
	bool dirL_frameBufferCreated = false;
	float* mousePointedData = nullptr; //Melyik j�t�kmez�re mutat az eg�r
	bool shotReceived = false; //K�ld�tt-e m�r l�v�st a szerver nek�nk
	bool shotSent = false; //K�ldt�nk-e m�r l�v�st
	int shipSizeInput = 0; //Mekkora haj�t k�sz�l�nk lerakni
	int winnerPlayerNum = 0; //Nyertes j�t�kos+boolk�nt is m�k�dik ha senki nem nyert m�g
	Sint32 beforeRotationX = 0; //Forgat�s el�tti X koord
	Sint32 beforeRotationY = 0; //Forgat�s el�tti Y koord

	int viewportWidth; //Ablak canvasz�nak sz�less�ge
	int viewportHeight; //Ablak canvasz�nak magass�ga
	gCamera* cam_mainCamera; //Kamera

};

