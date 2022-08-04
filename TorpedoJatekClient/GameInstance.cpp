#include "GameInstance.h"

//Kamera és más változók inicializálása
GameInstance::GameInstance(int viewport_w, int viewport_h) : viewportWidth(viewport_w),viewportHeight(viewport_h),
	cam_mainCamera(new gCamera(glm::vec3(0, 20.0f, 20.0f), mountain->getHeightY()))
{
	terrain = new Terrain(sea->getSeaTileRow(), mountain->getHeightY());
	cam_mainCamera->SetBoundaries(glm::vec3(
		terrain->getTerrainScale() * terrain->getGroundScaleXZ() / cameraRestraintXZ,
		mountain->getHeightY() * cameraRestraintY * TorpedoGLOBAL::Scale, 
		terrain->getTerrainScale() * terrain->getGroundScaleXZ() / cameraRestraintXZ));
	cam_mainCamera->SetProj(fieldOfView, viewportWidth / static_cast<float>(viewportHeight), 0.001f, viewDistance);
	
	mousePointedData = new float[4];
	mousePointedData[3] = 0.0f; //ez hogy legyen alapértéke mikor kell neki 3d pickinghez

	gameState = GameState::INITIAL;
}

//Memória felszabadítás
GameInstance::~GameInstance()
{
	vb_fbo.Clean();
	sh_default.Clean();
	sh_dirLight.Clean();
	
	delete[] mousePointedData;
	delete skybox;
	delete mountain;
	delete terrain;

	delete playerFleet;
	delete enemyFleet;
	delete sea;
	
	delete textHandler;
	delete eventHandler;
	delete gameLogic;
	delete cam_mainCamera;

	if (dirL_frameBufferCreated)
	{
		glDeleteRenderbuffers(1, &dirL_depthStencilBuffer);
		glDeleteTextures(1, &dirL_colorBuffer);
		glDeleteFramebuffers(1, &dirL_frameBuffer);
	}
}

//Játékmenet inicializálása
bool GameInstance::Init(std::string ip, std::string port)
{
	textHandler = new TextHandler();
	gameLogic = new GameLogic(*playerFleet, *enemyFleet, *sea, *textHandler);
	gameLogic->Init(ip, port);
	eventHandler = new EventHandler();

	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CreateFrameBuffer(viewportWidth, viewportHeight);
	vb_fbo.AddAttribute(0, 2); //position
	vb_fbo.AddAttribute(1, 2); //textcoord

	vb_fbo.AddData(0, -1, -1);
	vb_fbo.AddData(0, 1, -1);
	vb_fbo.AddData(0, -1, 1);
	vb_fbo.AddData(0, 1, 1);

	vb_fbo.AddData(1, 0, 0);
	vb_fbo.AddData(1, 1, 0);
	vb_fbo.AddData(1, 0, 1);
	vb_fbo.AddData(1, 1, 1);
	vb_fbo.InitBuffers();

	terrain->Init();
	mountain->Init();
	skybox->Init();

	sh_default.AttachShader(GL_VERTEX_SHADER, "Shaders/default.vert");
	sh_default.AttachShader(GL_FRAGMENT_SHADER, "Shaders/default.frag");
	sh_default.BindAttribLoc(0, "vs_in_pos");
	sh_default.BindAttribLoc(1, "vs_in_tex");
	if (!sh_default.LinkProgram()) {
		std::cout << "[Shader_Link]Error during Shader compilation: sh_default" << std::endl;
		return false;
	}

	sh_dirLight.AttachShader(GL_VERTEX_SHADER, "Shaders/dirLight.vert");
	sh_dirLight.AttachShader(GL_FRAGMENT_SHADER, "Shaders/dirLight.frag");
	sh_dirLight.BindAttribLoc(0, "vs_in_pos");
	sh_dirLight.BindAttribLoc(1, "vs_in_color");
	sh_dirLight.BindAttribLoc(2, "vs_in_normal");
	sh_dirLight.BindAttribLoc(3, "vs_in_tex");
	if (!sh_dirLight.LinkProgram())
	{
		std::cout << "[Shader_Link]Error during Shader compilation: sh_dirLight" << std::endl;
		return false;
	}

	return true;
}

//Adatok frissítése minden kirajzolásnál
void GameInstance::Update()
{
	//mozgatások/animációk
	static Uint32 lastTime = SDL_GetTicks();
	float deltaTime = (SDL_GetTicks() - lastTime) / 1000.0f;
	cam_mainCamera->Update(deltaTime);
	sea->Update(deltaTime);
	playerFleet->Update(deltaTime);
	enemyFleet->Update(deltaTime);
	eventHandler->Update(deltaTime, cam_mainCamera->GetEye());
	if(!TorpedoGLOBAL::Debug) {
		HandleGameState();
	}
	lastTime = SDL_GetTicks();
}

void GameInstance::HandleGameState() 
{
	//Real-time backend frissítések
	//Hajó lerakás szöveg és Játék vége szöveg
	if (!outputWritten) {
		if (isError) {
			if (gameState == GameState::SHIP_SIZE_INPUT) {
				gameLogic->DisplayError(gameState, shipSizeInput);
				shipSizeInput = 0;
				gameState = GameState::INITIAL;
			}
		}

		if (gameState == GameState::MATCH_ENDING) {
			gameLogic->DisplayMessage(gameState, winnerPlayerNum);
		}
		else {
			gameLogic->DisplayMessage(gameState, shipSizeInput);
		}
		if (gameState == GameState::INITIAL) {
			gameState = GameState::SHIP_SIZE_INPUT;
		}
		outputWritten = true;
	}

	//Játék indításánál mi kezdünk-e
	if (gameState == GameState::STARTING_MATCH) {
		if (gameLogic->CheckStartSignal()) {
			textHandler->Clear();
			if (gameLogic->getPlayerNum() == 1) {
				gameState = GameState::SHOOTING_AT_ENEMY;
			}
			else if (gameLogic->getPlayerNum() == 2) {
				gameState = GameState::GETTING_SHOT;
			}
			outputWritten = false;
		}
	} //Lövést kapunk
	else if (gameState == GameState::GETTING_SHOT) {
		if (shotReceived && !eventHandler->IsProjectileAnimation()) {
			gameState = GameState::SHOOTING_AT_ENEMY;
			shotReceived = false;
			outputWritten = false;
		}
		else if (!shotReceived) {
			PlayTile* pShotTile = gameLogic->GetShoot();
			if (pShotTile) {
				shotReceived = true;
				eventHandler->FireProjectile(*enemyFleet, *pShotTile);
			}
		}
		winnerPlayerNum = gameLogic->CheckVictoryState();
		if (winnerPlayerNum) {
			gameState = GameState::MATCH_ENDING;
		}
	}//Lövést adunk
	else if (shotSent && gameState == GameState::SHOOTING_AT_ENEMY) {
		if (!eventHandler->IsProjectileAnimation()) {
			gameState = GameState::GETTING_SHOT;
			shotSent = false;
			outputWritten = false;
		}
	}//Játék vége
	else if (!stopOutput && gameState == GameState::MATCH_ENDING) {
		if (!eventHandler->IsProjectileAnimation()) {
			outputWritten = false;
			stopOutput = true;
		}
	}
}

//Rajzolási hívás
void GameInstance::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, dirL_frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	sea->PreProcess(*cam_mainCamera);
	glReadPixels(mouseX, viewportHeight - mouseY - 1, 1, 1, GL_RGBA, GL_FLOAT, (void*)mousePointedData);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);

	sh_dirLight.On();
	mountain->Draw(*cam_mainCamera, sh_dirLight);
	playerFleet->Draw(*cam_mainCamera, sh_dirLight);
	enemyFleet->Draw(*cam_mainCamera, sh_dirLight);
	sh_dirLight.Off();

	skybox->Draw(*cam_mainCamera);

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	sh_dirLight.On();
	terrain->Draw(*cam_mainCamera, sh_dirLight);
	sh_dirLight.Off();

	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	
	sea->Draw(*cam_mainCamera);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0xFF);

	sea->OutlineDraw(*cam_mainCamera, mousePointedData[3]);
	
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sh_default.On();
	sh_default.SetTexture("quadTexture", 0, dirL_colorBuffer);
	vb_fbo.On();
	vb_fbo.Draw(GL_TRIANGLE_STRIP, 0, 4);
	vb_fbo.Off();
	sh_default.Off();

	textHandler->Render();

}

//true-val tér vissza ha be akarjuk zárni a programot magasabb szintrõl
bool GameInstance::KeyboardDown(SDL_KeyboardEvent& key)
{
	cam_mainCamera->KeyboardDown(key);

	//Kilépés
	if ((key.keysym.sym == SDLK_ESCAPE) && TorpedoGLOBAL::Debug) {
		return true;
	}
	if((key.keysym.sym == SDLK_ESCAPE) &&
		(gameState == GameState::SHOOTING_AT_ENEMY || gameState == GameState::GETTING_SHOT || gameState == GameState::STARTING_MATCH
			|| gameState == GameState::MATCH_ENDING)) {
		if (gameState != GameState::MATCH_ENDING) {
			gameLogic->QuitGame();
		}
		return true;
	}
	//Hang némítása
	if (key.keysym.sym == SDLK_m) {
		eventHandler->SwitchVolume();
	}
	//Kiválasztjuk hogy mekkora hajót készülünk lerakni
	if (gameState == GameState::SHIP_SIZE_INPUT) {
		switch (key.keysym.sym)
		{
		case SDLK_1:
			shipSizeInput = 1;
			break;
		case SDLK_2:
			shipSizeInput = 2;
			break;
		case SDLK_3:
			shipSizeInput = 3;
			break;
		case SDLK_4:
			shipSizeInput = 4;
			break;
		case SDLK_ESCAPE:
			gameLogic->QuitGame();
			return true;
			break;
		}
		if (shipSizeInput) {
			if (gameLogic->CheckForUnplacedShips(shipSizeInput)) {
				gameState = GameState::PLACING_SHIP;
			}
			else {
				isError = true;
			}
			outputWritten = false;
		}
	}

	//Tesztek DEBUGban
	if(TorpedoGLOBAL::Debug){
		//Lövés F - Süllyedés G 
		if (key.keysym.sym == SDLK_f) {
			if (!eventHandler->IsProjectileAnimation()) {
				eventHandler->FireProjectile(*playerFleet, sea->getTileByIndex(15, false));
				
			}
		}
		else if (key.keysym.sym == SDLK_g) {
			if (!playerFleet->getBattleShip().isVisible()) {
				playerFleet->getBattleShip().ResetForDEBUG();
			}
			else {
				playerFleet->getBattleShip().setDestroyed(true);
			}
		}
	}

	return false;
}

void GameInstance::KeyboardUp(SDL_KeyboardEvent& key)
{
	cam_mainCamera->KeyboardUp(key);
}

void GameInstance::MouseMove(SDL_MouseMotionEvent& mouse)
{
	mouseX = mouse.x;
	mouseY = mouse.y;

	cam_mainCamera->MouseMove(mouse);
}

void GameInstance::MouseDown(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT) {
		//Hajót teszünk mi le
		if (gameState == GameState::PLACING_SHIP) {
			//	if (SDL_GetRelativeMouseMode() == SDL_bool(false)) {
			//		SDL_SetRelativeMouseMode(SDL_bool(true));
			//	}
			if (gameLogic->PlaceAllyShip(static_cast<int>(mousePointedData[3]), shipSizeInput)) {
				if (gameLogic->CheckAnyUnplacedShipLeft()) {
					gameState = GameState::INITIAL;
				}
				else {
					gameLogic->SendFleetToServer();
					gameState = GameState::STARTING_MATCH;
				}
				shipSizeInput = 0;
				outputWritten = false;
			}
		}
		//Mi lövünk
		if (gameState == GameState::SHOOTING_AT_ENEMY) {
			if (!eventHandler->IsProjectileAnimation()) {
				PlayTile* pShotTile = gameLogic->Shoot(static_cast<int>(mousePointedData[3]));
				if (pShotTile) {
					eventHandler->FireProjectile(*playerFleet, *pShotTile);
					shotSent = true;

					winnerPlayerNum = gameLogic->CheckVictoryState();
					if (winnerPlayerNum) {
						gameState = GameState::MATCH_ENDING;
					}
				}
			}
		}
	}
}

void GameInstance::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void GameInstance::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

//Ablak átméretezéskor hívódik
void GameInstance::Resize(int width, int height)
{
	glViewport(0, 0, width, height);

	viewportWidth = width;
	viewportHeight = height;

	cam_mainCamera->Resize(width, height, fieldOfView, viewDistance);
	CreateFrameBuffer(width, height);
}

//Custom Framebuffer létrehozása
void GameInstance::CreateFrameBuffer(int width, int height) 
{
	if (dirL_frameBufferCreated)
	{
		glDeleteRenderbuffers(1, &dirL_depthStencilBuffer);
		glDeleteTextures(1, &dirL_colorBuffer);
		glDeleteFramebuffers(1, &dirL_frameBuffer);
	}

	glGenFramebuffers(1, &dirL_frameBuffer);
	glGenTextures(1, &dirL_colorBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, dirL_frameBuffer);
	glBindTexture(GL_TEXTURE_2D, dirL_colorBuffer);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dirL_colorBuffer, 0);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cout << "[Create_FBO]Error creating color attachment" << std::endl;
		char ch; std::cin >> ch;
		exit(1);
	}

	glGenRenderbuffers(1, &dirL_depthStencilBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, dirL_depthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dirL_depthStencilBuffer);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cout << "[Create_FBO]Error creating depth/stencil attachment" << std::endl;
		char ch;
		std::cin >> ch;
		exit(1);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[Create_FBO]Incomplete framebuffer (";
		switch (status) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "GL_FRAMEBUFFER_UNSUPPORTED";
			break;
		}
		std::cout << ")" << std::endl;
		char ch;
		std::cin >> ch;
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	dirL_frameBufferCreated = true;
}