
#include "GameInstance.h"

GameInstance::GameInstance(void)
{
	/*
	m_coneTextureID = 0;
	m_coneNormalMapID = 0;

	m_mesh = 0;*/
}

GameInstance::~GameInstance(void)
{
	if (!TorpedoGLOBAL::Debug) {
		SDL_WaitThread(inputThread, nullptr);
	}
}

//Játékmenet inicializálása
bool GameInstance::Init()
{
	mapSize = gameLogic.Init(&playerFleet, &enemyFleet, &sea);
	gameLogic.InitGame();

	if (!TorpedoGLOBAL::Debug) {
		inputThread = SDL_CreateThread(threadFunction, "inputThread", (void*)this);
	}
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	skybox.Init();
	mountain.Init();
	terrain.Init();

	sh_dirLight.AttachShader(GL_VERTEX_SHADER, "Shaders/dirLight.vert");
	sh_dirLight.AttachShader(GL_FRAGMENT_SHADER, "Shaders/dirLight.frag");
	sh_dirLight.BindAttribLoc(0, "vs_in_pos");
	sh_dirLight.BindAttribLoc(1, "vs_in_color");
	sh_dirLight.BindAttribLoc(2, "vs_in_normal");
	//m_program.BindAttribLoc(3, "vs_in_tex0");
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

	cam_mainCamera.SetProj(45.0f, 800.0f / 600.0f, 0.01f, 1000.0f);
	/*
	// textúrák betöltése
	m_coneTextureID = TextureFromFile("Resources/Textures/cylinder_texture.bmp");
	m_coneNormalMapID = TextureFromFile("Resources/Normals/cylinder_normal.bmp");

	// mesh betöltés
	m_mesh = ObjParser::parse("Resources/suzanne.obj");
	m_mesh->initBuffers();
	*/

	return true;
}

void GameInstance::Clean()
{
	/*glDeleteTextures(1, &m_coneTextureID);
	glDeleteTextures(1, &m_coneNormalMapID);*/

	sh_dirLight.Clean();
	sh_skybox.Clean();
}

//A thread hívja meg ezt a függvényt,hogy lehessen meccs közbe gépelni consoleba
int GameInstance::threadFunction(void *ptr)
{
	if (ptr) {
		GameInstance* pointr = static_cast<GameInstance *>(ptr);
		pointr->gameLogic.StartMatch(pointr->sea.getTiles(true), pointr->sea.getTiles(false));
	}
	std::cout << "The match is over,input thread stopped." << std::endl;
	return 1;

}

//Adatok frissítése minden kirajzolásnál
void GameInstance::Update()
{
	// kameramozgatáshoz
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	cam_mainCamera.Update(delta_time);
	last_time = SDL_GetTicks();

}

//Rajzolási hívás
void GameInstance::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// megjelenítés módja
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
	
	sea.Draw(cam_mainCamera, sh_dirLight);
	sh_dirLight.Off();

	sh_skybox.On();
	skybox.Draw(cam_mainCamera, sh_skybox);
	sh_skybox.Off();
}

void GameInstance::KeyboardDown(SDL_KeyboardEvent& key)
{
	cam_mainCamera.KeyboardDown(key);
}

void GameInstance::KeyboardUp(SDL_KeyboardEvent& key)
{
	cam_mainCamera.KeyboardUp(key);
}

void GameInstance::MouseMove(SDL_MouseMotionEvent& mouse)
{
	cam_mainCamera.MouseMove(mouse);
}

void GameInstance::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void GameInstance::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void GameInstance::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

//Ablak átméretezéskor hívódik
void GameInstance::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	cam_mainCamera.Resize(_w, _h);
}