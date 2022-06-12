#include "GameInstance.h"

//Kamera és más változók inicializálása
GameInstance::GameInstance(float viewportW, float viewportH) : viewportWidth(viewportW),viewportHeight(viewportH),
	cam_mainCamera(glm::vec3(0,20.0f,20.0f))
{
	cam_mainCamera.SetBoundaries(terrain.getTerrainScale() * Ground::getScaleXZ() / 3.0f * TorpedoGLOBAL::Scale,
		Mountain::getHeight() * 4.0f * TorpedoGLOBAL::Scale, 
		terrain.getTerrainScale() * Ground::getScaleXZ() / 3.0f * TorpedoGLOBAL::Scale);
	cam_mainCamera.SetProj(fieldOfView, viewportWidth / viewportHeight, 0.01f, viewDistance);
	
	mousePointedData = new float[4];
	mousePointedData[3] = 0.0f; //ez hogy legyen alapértéke mikor kell neki 3d pickinghez

	gameState = GameState::INITIAL;
	isError = false;
	outputWritten = false;

	stateRelatedData.push_back(NULL);
	stateRelatedData.push_back(NULL);
	stateRelatedData.push_back(&shipSizeInput);
	stateRelatedData.push_back(&shipSizeInput);

	//temp- nemjó eza DisplayMessage-staterelatedData megoldás annyira,függ enum mérettõ
	stateRelatedData.push_back(&shipSizeInput);
	stateRelatedData.push_back(&shipSizeInput);
	stateRelatedData.push_back(&shipSizeInput);
	stateRelatedData.push_back(&shipSizeInput);
	stateRelatedData.push_back(&winnerPlayerNum);
}

//Memória felszabadítás
GameInstance::~GameInstance(void)
{
	Mix_FreeChunk(cannonFireSound);

	delete[] mousePointedData;

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
	mapSize = gameLogic.Init(&playerFleet, &enemyFleet, &sea);
	gameLogic.InitGame();

	cannonFireSound = Mix_LoadWAV("Resources/Audio/cannonFire.wav");
	if (!cannonFireSound) {
		printf("Mix_LoadWAV error: %s\n", SDL_GetError());
	}
	if (!TorpedoGLOBAL::AudioEnabled) {
		Mix_VolumeChunk(cannonFireSound, 0);
	}

	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CreateFrameBuffer(viewportWidth, viewportHeight);
	m_fbo_vbo.AddAttribute(0, 3);
	m_fbo_vbo.AddAttribute(1, 2);
	m_fbo_vbo.AddData(0, -1, -1, 0);
	m_fbo_vbo.AddData(0, 1, -1, 0);
	m_fbo_vbo.AddData(0, -1, 1, 0);
	m_fbo_vbo.AddData(0, 1, 1, 0);
	m_fbo_vbo.AddData(1, 0, 0);
	m_fbo_vbo.AddData(1, 1, 0);
	m_fbo_vbo.AddData(1, 0, 1);
	m_fbo_vbo.AddData(1, 1, 1);
	m_fbo_vbo.InitBuffers();

	skybox.Init();
	mountain.Init();
	terrain.Init();

	sh_default.AttachShader(GL_VERTEX_SHADER, "Shaders/default.vert");
	sh_default.AttachShader(GL_FRAGMENT_SHADER, "Shaders/default.frag");
	sh_default.BindAttribLoc(0, "vs_in_pos");
	sh_default.BindAttribLoc(1, "vs_in_tex");
	if (!sh_default.LinkProgram()) {
		return false;
	}

	sh_playtile.AttachShader(GL_VERTEX_SHADER, "Shaders/playtile.vert");
	sh_playtile.AttachShader(GL_FRAGMENT_SHADER, "Shaders/playtile.frag");
	sh_playtile.BindAttribLoc(0, "vs_in_pos");
	sh_playtile.BindAttribLoc(1, "vs_in_col");
	if (!sh_playtile.LinkProgram()) {
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
		return false;
	}

	sh_skybox.AttachShader(GL_VERTEX_SHADER, "Shaders/skybox.vert");
	sh_skybox.AttachShader(GL_FRAGMENT_SHADER, "Shaders/skybox.frag");
	sh_skybox.BindAttribLoc(0, "vs_in_pos");
	if (!sh_skybox.LinkProgram()) {
		return false;
	}

	/// mesh betöltés
	///m_mesh = ObjParser::parse("Resources/suzanne.obj");
	///m_mesh->initBuffers();

	return true;
}

void GameInstance::Clean()
{
	sh_default.Clean();
	sh_dirLight.Clean();
	sh_skybox.Clean();
}



//Adatok frissítése minden kirajzolásnál
void GameInstance::Update()
{
	//mozgatások/animációk
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	cam_mainCamera.Update(delta_time);
	sea.Update(delta_time);
	playerFleet.Update(delta_time);
	enemyFleet.Update(delta_time);
	eventHandler.Update(delta_time, cam_mainCamera.GetEye());
	last_time = SDL_GetTicks();

	//Real-time backend frissítések
	if (!TorpedoGLOBAL::Debug && !outputWritten) {
		if (isError) {
			if (gameState == GameState::SHIP_SIZE_INPUT) {
				gameLogic.DisplayError(gameState, stateRelatedData.at(gameState));
				shipSizeInput = 0;
				gameState = GameState::INITIAL;
			}
		}

		gameLogic.DisplayMessage(gameState, stateRelatedData.at(gameState));
		if (gameState == GameState::INITIAL) {
			gameState = GameState::SHIP_SIZE_INPUT;
		}
		outputWritten = true;
	}

	if (gameState == GameState::STARTING_MATCH) {
		if (gameLogic.CheckStartSignal()) {
			if (gameLogic.getPlayerNum() == 1) {
				gameState = GameState::SHOOTING_AT_ENEMY;
			}
			else if (gameLogic.getPlayerNum() == 2) {
				gameState = GameState::GETTING_SHOT;
			}
			outputWritten = false;
		}
	}
	else if (gameState == GameState::GETTING_SHOT) {
		if (shotReceived && !eventHandler.IsProjectileAnimation()) {
			gameState = GameState::SHOOTING_AT_ENEMY;
			shotReceived = false;
			outputWritten = false;
		}
		else if (!shotReceived){
			PlayTile* shotTile = gameLogic.GetShoot();
			if (shotTile) {
				shotReceived = true;
				//std::cout << "YOLOOO:   " << shotTile->getIndex() << std::endl;
				eventHandler.FireProjectile(enemyFleet, shotTile);

				float scannedDistance = 40.0f;
				float len = glm::length(glm::vec3(cam_mainCamera.GetEye().x - enemyFleet.getBattleShip().getShipTranslate().x
					, 0
					, cam_mainCamera.GetEye().z - enemyFleet.getBattleShip().getShipTranslate().z));
				int ds = (len > scannedDistance ? (scannedDistance - 1) : len) * 256 / scannedDistance;
				Mix_SetDistance(1, ds);
				Mix_PlayChannel(1, cannonFireSound, 0);
			}
		}
		winnerPlayerNum = gameLogic.CheckVictoryState();
		if (winnerPlayerNum && !eventHandler.IsProjectileAnimation()) {
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
	sea.PreProcess(cam_mainCamera, sh_playtile);
	sh_playtile.Off();
	glReadPixels(mouseX, viewportHeight - mouseY - 1, 1, 1, GL_RGBA, GL_FLOAT, (void*)mousePointedData);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);

	sh_dirLight.On();
	mountain.Draw(cam_mainCamera, sh_dirLight);
	terrain.Draw(cam_mainCamera, sh_dirLight);
	playerFleet.Draw(cam_mainCamera, sh_dirLight);
	enemyFleet.Draw(cam_mainCamera, sh_dirLight);
	sh_dirLight.Off();

	sh_skybox.On();
	skybox.Draw(cam_mainCamera, sh_skybox);
	sh_skybox.Off();

	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	
	sh_dirLight.On();
	sea.Draw(cam_mainCamera, sh_dirLight);
	sh_dirLight.Off();

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0xFF);

	sh_playtile.On();
	sea.OutlineDraw(cam_mainCamera, sh_playtile, mousePointedData[3]);
	sh_playtile.Off();
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sh_default.On();
	sh_default.SetTexture("quadTexture", 0, dirL_colorBuffer);
	m_fbo_vbo.On();
	m_fbo_vbo.Draw(GL_TRIANGLE_STRIP, 0, 4);
	m_fbo_vbo.Off();
	sh_default.Off();
}

//true-val tér vissza ha be akarjuk zárni a programot alacsonyabb szintrõl
bool GameInstance::KeyboardDown(SDL_KeyboardEvent& key)
{
	cam_mainCamera.KeyboardDown(key);

	if ((key.keysym.sym == SDLK_ESCAPE) && TorpedoGLOBAL::Debug) {
		return 1;
	}

	if((key.keysym.sym == SDLK_ESCAPE) &&
		(gameState == GameState::SHOOTING_AT_ENEMY || gameState == GameState::GETTING_SHOT || gameState == GameState::STARTING_MATCH
			|| gameState == GameState::MATCH_ENDING)) {
		if (gameState != GameState::MATCH_ENDING) {
			gameLogic.StopGame();
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
			gameLogic.StopGame();
			return 1;
			break;
		}
		if (shipSizeInput) {
			if (gameLogic.CheckForUnplacedShips(shipSizeInput)) {
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
			if (!eventHandler.IsProjectileAnimation()) {
				eventHandler.FireProjectile(playerFleet, &sea.getTileByIndex(115));
				
				float scannedDistance = 40.0f;
				float len = glm::length(glm::vec3(cam_mainCamera.GetEye().x - playerFleet.getBattleShip().getShipTranslate().x
					, 0
					, cam_mainCamera.GetEye().z - playerFleet.getBattleShip().getShipTranslate().z));
				int ds = (len > scannedDistance ? (scannedDistance - 1) : len) * 256 / scannedDistance;
				Mix_SetDistance(1, ds);
				Mix_PlayChannel(1, cannonFireSound, 0);
			}
		}
		else if (key.keysym.sym == SDLK_g) {
			if (!playerFleet.getBattleShip().isVisible()) {
				playerFleet.getBattleShip().ResetForDEBUG();
			}
			else {
				playerFleet.getBattleShip().setDestroyed(true);
			}
		}
	}

	return 0;
}

void GameInstance::KeyboardUp(SDL_KeyboardEvent& key)
{
	cam_mainCamera.KeyboardUp(key);
}

void GameInstance::MouseMove(SDL_MouseMotionEvent& mouse)
{
	mouseX = mouse.x;
	mouseY = mouse.y;

	cam_mainCamera.MouseMove(mouse);
}

void GameInstance::MouseDown(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT) {
		if (gameState == GameState::PLACING_SHIP) {
			//	if (SDL_GetRelativeMouseMode() == SDL_bool(false)) {
			//		SDL_SetRelativeMouseMode(SDL_bool(true));
			//	}
			if (gameLogic.PlaceShip(static_cast<int>(mousePointedData[3]), shipSizeInput)) {
				if (gameLogic.CheckAnyUnplacedShipLeft()) {
					gameState = GameState::INITIAL;
				}
				else {
					gameLogic.SendFleetToServer();
					gameState = GameState::STARTING_MATCH;
				}
				shipSizeInput = 0;
				outputWritten = false;
			}
		}

		if (gameState == GameState::SHOOTING_AT_ENEMY) {
			if (!eventHandler.IsProjectileAnimation()) {
				PlayTile* shotTile = gameLogic.Shoot(static_cast<int>(mousePointedData[3]));
				if (shotTile) {
					eventHandler.FireProjectile(playerFleet, shotTile);
					float scannedDistance = 40.0f;
					float len = glm::length(glm::vec3(cam_mainCamera.GetEye().x - playerFleet.getBattleShip().getShipTranslate().x
						, 0
						, cam_mainCamera.GetEye().z - playerFleet.getBattleShip().getShipTranslate().z));
					int ds = (len > scannedDistance ? (scannedDistance - 1) : len) * 256 / scannedDistance;
					Mix_SetDistance(1, ds);
					Mix_PlayChannel(1, cannonFireSound, 0);

					winnerPlayerNum = gameLogic.CheckVictoryState();
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
void GameInstance::Resize(int w, int h)
{
	glViewport(0, 0, w, h);

	viewportWidth = w;
	viewportHeight = h;

	cam_mainCamera.Resize(w, h, fieldOfView, viewDistance);
	CreateFrameBuffer(w, h);
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
		std::cout << "Error creating color attachment" << std::endl;
		char ch; std::cin >> ch;
		exit(1);
	}

	glGenRenderbuffers(1, &dirL_depthStencilBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, dirL_depthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dirL_depthStencilBuffer);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cout << "Error creating depth/stencil attachment" << std::endl;
		char ch;
		std::cin >> ch;
		exit(1);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Incomplete framebuffer (";
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