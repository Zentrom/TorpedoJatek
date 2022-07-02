#include "GameInstance.h"

//Kamera és más változók inicializálása
GameInstance::GameInstance(float viewport_w, float viewport_h) : viewportWidth(viewport_w),viewportHeight(viewport_h),
	cam_mainCamera(new gCamera(glm::vec3(0, 20.0f, 20.0f), mountain->getHeightY()))
{
	terrain = new Terrain(sea->getSeaTileRow(), mountain->getHeightY());
	cam_mainCamera->SetBoundaries(glm::vec3(
		terrain->getTerrainScale() * terrain->getGroundScaleXZ() / cameraRestraintXZ,
		mountain->getHeightY() * cameraRestraintY * TorpedoGLOBAL::Scale, 
		terrain->getTerrainScale() * terrain->getGroundScaleXZ() / cameraRestraintXZ));
	cam_mainCamera->SetProj(fieldOfView, viewportWidth / viewportHeight, 0.001f, viewDistance);
	
	mousePointedData = new float[4];
	mousePointedData[3] = 0.0f; //ez hogy legyen alapértéke mikor kell neki 3d pickinghez

	gameState = GameState::INITIAL;
}

//Memória felszabadítás
GameInstance::~GameInstance()
{
	//Mix_FreeChunk(cannonFireSound);
	//delete cannonFireSound; NEMKELL elrontja

	vb_fbo.Clean();
	sh_default.Clean();
	sh_playtile.Clean();
	sh_dirLight.Clean();
	
	delete[] mousePointedData;
	delete skybox;
	delete mountain;
	delete terrain;

	delete playerFleet;
	delete enemyFleet;
	delete sea;
	
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
bool GameInstance::Init()
{
	gameLogic = new GameLogic(*playerFleet, *enemyFleet, *sea);
	gameLogic->Init();

	//cannonFireSound = Mix_LoadWAV("Resources/Audio/cannonFire.wav");
	//if (!cannonFireSound) {
	//	printf("Mix_LoadWAV error: %s\n", SDL_GetError());
	//}
	//if (!TorpedoGLOBAL::AudioEnabled) {
	//	Mix_VolumeChunk(cannonFireSound, 0);
	//}

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

	sh_playtile.AttachShader(GL_VERTEX_SHADER, "Shaders/playtile.vert");
	sh_playtile.AttachShader(GL_FRAGMENT_SHADER, "Shaders/playtile.frag");
	sh_playtile.BindAttribLoc(0, "vs_in_pos");
	sh_playtile.BindAttribLoc(1, "vs_in_col");
	if (!sh_playtile.LinkProgram()) {
		std::cout << "[Shader_Link]Error during Shader compilation: sh_playtile" << std::endl;
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
	lastTime = SDL_GetTicks();

	if(!TorpedoGLOBAL::Debug) {
		HandleGameState();
	}
}

void GameInstance::HandleGameState() 
{
	//Real-time backend frissítések
	//Hajó lerakás szöveg és Játék vége szöveg
	if (!outputWritten) {
		if (isError) {
			if (gameState == GameState::SHIP_SIZE_INPUT) {
				gameLogic->DisplayError(gameState, shipSizeInput);//stateRelatedData.at(gameState));
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
			PlayTile* shotTile = gameLogic->GetShoot();
			if (shotTile) {
				shotReceived = true;
				eventHandler->FireProjectile(enemyFleet, shotTile);

				//float scannedDistance = 40.0f;
				//float len = glm::length(glm::vec3(cam_mainCamera->GetEye().x - enemyFleet->getBattleShip().getShipTranslate().x
				//	, 0
				//	, cam_mainCamera->GetEye().z - enemyFleet->getBattleShip().getShipTranslate().z));
				//int ds = (len > scannedDistance ? (scannedDistance - 1) : len) * 256 / scannedDistance;
				//Mix_SetDistance(1, ds);
				//Mix_PlayChannel(1, cannonFireSound, 0);
			}
		}
		winnerPlayerNum = gameLogic->CheckVictoryState();
		if (winnerPlayerNum && !eventHandler->IsProjectileAnimation()) {
			gameState = GameState::MATCH_ENDING;
			outputWritten = false;
		}
	}
}

//Rajzolási hívás
void GameInstance::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, dirL_frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	sh_playtile.On();
	sea->PreProcess(*cam_mainCamera, sh_playtile);
	sh_playtile.Off();
	glReadPixels(mouseX, viewportHeight - mouseY - 1, 1, 1, GL_RGBA, GL_FLOAT, (void*)mousePointedData);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);

	sh_dirLight.On();
	mountain->Draw(*cam_mainCamera, sh_dirLight);
	terrain->Draw(*cam_mainCamera, sh_dirLight);
	playerFleet->Draw(*cam_mainCamera, sh_dirLight);
	enemyFleet->Draw(*cam_mainCamera, sh_dirLight);
	sh_dirLight.Off();

	skybox->Draw(*cam_mainCamera);

	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	
	sh_dirLight.On();
	sea->Draw(*cam_mainCamera, sh_dirLight);
	sh_dirLight.Off();

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0xFF);

	sh_playtile.On();
	sea->OutlineDraw(*cam_mainCamera, sh_playtile, mousePointedData[3]);
	sh_playtile.Off();
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
}

//true-val tér vissza ha be akarjuk zárni a programot magasabb szintrõl
bool GameInstance::KeyboardDown(SDL_KeyboardEvent& key)
{
	cam_mainCamera->KeyboardDown(key);

	if ((key.keysym.sym == SDLK_ESCAPE) && TorpedoGLOBAL::Debug) {
		return 1;
	}

	if((key.keysym.sym == SDLK_ESCAPE) &&
		(gameState == GameState::SHOOTING_AT_ENEMY || gameState == GameState::GETTING_SHOT || gameState == GameState::STARTING_MATCH
			|| gameState == GameState::MATCH_ENDING)) {
		if (gameState != GameState::MATCH_ENDING) {
			gameLogic->StopGame();
		}
		return 1;
	}

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
			gameLogic->StopGame();
			return 1;
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
				eventHandler->FireProjectile(playerFleet, &sea->getTileByIndex(15, false));
				
				//float scannedDistance = 40.0f;
				//float len = glm::length(glm::vec3(cam_mainCamera->GetEye().x - playerFleet->getBattleShip().getShipTranslate().x
				//	, 0
				//	, cam_mainCamera->GetEye().z - playerFleet->getBattleShip().getShipTranslate().z));
				//int ds = (len > scannedDistance ? (scannedDistance - 1) : len) * 256 / scannedDistance;
				//Mix_SetDistance(1, ds);
				//Mix_PlayChannel(1, cannonFireSound, 0);
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

	return 0;
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
		if (gameState == GameState::PLACING_SHIP) {
			//	if (SDL_GetRelativeMouseMode() == SDL_bool(false)) {
			//		SDL_SetRelativeMouseMode(SDL_bool(true));
			//	}
			if (gameLogic->PlaceShip(static_cast<int>(mousePointedData[3]), shipSizeInput)) {
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

		if (gameState == GameState::SHOOTING_AT_ENEMY) {
			if (!eventHandler->IsProjectileAnimation()) {
				PlayTile* shotTile = gameLogic->Shoot(static_cast<int>(mousePointedData[3]));
				if (shotTile) {
					eventHandler->FireProjectile(playerFleet, shotTile);
					//float scannedDistance = 40.0f;
					//float len = glm::length(glm::vec3(cam_mainCamera->GetEye().x - playerFleet->getBattleShip().getShipTranslate().x
					//	, 0
					//	, cam_mainCamera->GetEye().z - playerFleet->getBattleShip().getShipTranslate().z));
					//int ds = (len > scannedDistance ? (scannedDistance - 1) : len) * 256 / scannedDistance;
					//Mix_SetDistance(1, ds);
					//Mix_PlayChannel(1, cannonFireSound, 0);

					winnerPlayerNum = gameLogic->CheckVictoryState();
					if (winnerPlayerNum) {
						gameState = GameState::MATCH_ENDING;
					}
					else {
						gameState = GameState::GETTING_SHOT;
					}
					outputWritten = false;
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