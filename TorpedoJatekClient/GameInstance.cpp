#include "GameInstance.h"

//Kamera �s m�s v�ltoz�k inicializ�l�sa
GameInstance::GameInstance(float viewportW, float viewportH) : viewportWidth(viewportW),viewportHeight(viewportH),
	cam_mainCamera(glm::vec3(0,20.0f,20.0f))
{
	cam_mainCamera.SetBoundaries(terrain.getTerrainScale() * Ground::getScaleXZ() / 3.0f * TorpedoGLOBAL::Scale,
		Mountain::getHeight() * 4.0f * TorpedoGLOBAL::Scale, 
		terrain.getTerrainScale() * Ground::getScaleXZ() / 3.0f * TorpedoGLOBAL::Scale);
	cam_mainCamera.SetProj(fieldOfView, viewportWidth / viewportHeight, 0.01f, viewDistance);
	
	mousePointedData = new float[4];
	mousePointedData[3] = 0.0f; //ez hogy legyen alap�rt�ke mikor kell neki 3d pickinghez

	gameState = GameState::INITIAL;
	isError = false;
	outputWritten = false;

	stateRelatedData.push_back(NULL);
	stateRelatedData.push_back(NULL);
	stateRelatedData.push_back(&shipSizeInput);
	stateRelatedData.push_back(&shipSizeInput);

	//temp- nemj� eza DisplayMessage-staterelatedData megold�s annyira,f�gg enum m�rett�
	stateRelatedData.push_back(&shipSizeInput);
	stateRelatedData.push_back(&shipSizeInput);
	stateRelatedData.push_back(&shipSizeInput);
	stateRelatedData.push_back(&shipSizeInput);
	stateRelatedData.push_back(&winnerPlayerNum);
}

//Mem�ria felszabad�t�s
GameInstance::~GameInstance(void)
{
	//if (!TorpedoGLOBAL::Debug) {
	//	SDL_WaitThread(inputThread, nullptr);
	//}

	delete[] mousePointedData;

	if (dirL_frameBufferCreated)
	{
		glDeleteRenderbuffers(1, &dirL_depthBuffer);
		glDeleteTextures(1, &dirL_colorBuffer);
		glDeleteFramebuffers(1, &dirL_frameBuffer);
	}
}

//J�t�kmenet inicializ�l�sa
bool GameInstance::Init()
{
	mapSize = gameLogic.Init(&playerFleet, &enemyFleet, &sea);
	gameLogic.InitGame();

	//gameLogic.StartMatch(sea.getTiles(true), sea.getTiles(false));
	//if (!TorpedoGLOBAL::Debug) {
	//	inputThread = SDL_CreateThread(threadFunction, "inputThread", (void*)this);
	//}
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
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

	/// mesh bet�lt�s
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

//A thread h�vja meg ezt a f�ggv�nyt,hogy lehessen meccs k�zbe g�pelni consoleba
//int GameInstance::threadFunction(void *ptr)
//{
//	if (ptr) {
//		GameInstance* pointr = static_cast<GameInstance *>(ptr);
//		pointr->gameLogic.StartMatch(pointr->sea.getTiles(true), pointr->sea.getTiles(false));
//	}
//	std::cout << "The match is over,input thread stopped." << std::endl;
//	return 1;
//
//}

//Adatok friss�t�se minden kirajzol�sn�l
void GameInstance::Update()
{
	//mozgat�sok/anim�ci�k
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	cam_mainCamera.Update(delta_time);
	sea.Update(delta_time);
	last_time = SDL_GetTicks();

	//Real-time backend friss�t�sek
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
		if (gameLogic.GetShoot()) {
			gameState = GameState::SHOOTING_AT_ENEMY;
			
			winnerPlayerNum = gameLogic.CheckVictoryState();
			if (winnerPlayerNum) {
				gameState = GameState::MATCH_ENDING;
			}

			outputWritten = false;
		}
	}


}

//Rajzol�si h�v�s
void GameInstance::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, dirL_frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// megjelen�t�s m�dja
	//if (is_filled) {
		//glEnable(GL_CULL_FACE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//}
	//else {
	//	glDisable(GL_CULL_FACE);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//}

	sh_dirLight.On();
	mountain.Draw(cam_mainCamera, sh_dirLight);
	terrain.Draw(cam_mainCamera, sh_dirLight);
	playerFleet.Draw(cam_mainCamera, sh_dirLight);
	enemyFleet.Draw(cam_mainCamera, sh_dirLight);
	sh_dirLight.Off();

	sh_skybox.On();
	skybox.Draw(cam_mainCamera, sh_skybox);
	sh_skybox.Off();

	sh_dirLight.On();
	sea.Draw(cam_mainCamera, sh_dirLight, mousePointedData[3]);
	sh_dirLight.Off();

	//glReadPixels(viewportWidth / 2, viewportHeight / 2, 1, 1, GL_RGBA, GL_FLOAT, (void*)mousePointedData);
	//std::cout << mousePointedData[3] << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sh_default.On();
	sh_default.SetTexture("quadTexture", 0, dirL_colorBuffer);
	m_fbo_vbo.On();
	m_fbo_vbo.Draw(GL_TRIANGLE_STRIP, 0, 4);
	m_fbo_vbo.Off();
	sh_default.Off();
}

//true-val t�r vissza ha be akarjuk z�rni a programot alacsonyabb szintr�l
bool GameInstance::KeyboardDown(SDL_KeyboardEvent& key)
{
	cam_mainCamera.KeyboardDown(key);

	if ((key.keysym.sym == SDLK_ESCAPE) && TorpedoGLOBAL::Debug) {
		return 1;
	}

	if((key.keysym.sym == SDLK_ESCAPE) &&
		(gameState == GameState::SHOOTING_AT_ENEMY || gameState == GameState::GETTING_SHOT
			|| gameState == GameState::MATCH_ENDING)) {
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
	return 0;
}

void GameInstance::KeyboardUp(SDL_KeyboardEvent& key)
{
	cam_mainCamera.KeyboardUp(key);
}

void GameInstance::MouseMove(SDL_MouseMotionEvent& mouse)
{
	cam_mainCamera.MouseMove(mouse);
	//std::cout << mouse.x << " and " << mouse.y << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, dirL_frameBuffer);
	glReadPixels(mouse.x, viewportHeight - mouse.y - 1, 1, 1, GL_RGBA, GL_FLOAT, (void*)mousePointedData);
	//std::cout << mousePointedData[3] << std::endl;
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
			if (gameLogic.Shoot(static_cast<int>(mousePointedData[3]))) {

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

void GameInstance::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void GameInstance::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

//Ablak �tm�retez�skor h�v�dik
void GameInstance::Resize(int w, int h)
{
	glViewport(0, 0, w, h);

	viewportWidth = w;
	viewportHeight = h;

	cam_mainCamera.Resize(w, h, fieldOfView, viewDistance);
	CreateFrameBuffer(w, h);
}

//Custom Framebuffer l�trehoz�sa
void GameInstance::CreateFrameBuffer(int width, int height) 
{
	if (dirL_frameBufferCreated)
	{
		glDeleteRenderbuffers(1, &dirL_depthBuffer);
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

	glGenRenderbuffers(1, &dirL_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, dirL_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dirL_depthBuffer);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cout << "Error creating depth attachment" << std::endl;
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