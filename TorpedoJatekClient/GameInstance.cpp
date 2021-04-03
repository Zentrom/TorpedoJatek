//#include <GL/GLU.h>
#include <math.h>

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
}



bool GameInstance::Init()
{
	gameLogic.Init();
	actPlayTiles = gameLogic.getActiveTiles();

	if (!TorpedoGLOBAL::Debug) {
		inputThread = SDL_CreateThread(threadFunction, "inputThread", (void*)this);
	}
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	mountain.Init();
	terrain.Init();


	//tile
	sea.Init();
	
	//ships
	fleet.Init(actPlayTiles);
	
	sh_dirLight.AttachShader(GL_VERTEX_SHADER, "Shaders/dirLight.vert");
	sh_dirLight.AttachShader(GL_FRAGMENT_SHADER, "Shaders/dirLight.frag");

	sh_dirLight.BindAttribLoc(0, "vs_in_pos");
	sh_dirLight.BindAttribLoc(1, "vs_in_color");
	//m_program.BindAttribLoc(2, "vs_in_normal");
	//m_program.BindAttribLoc(3, "vs_in_tex0");

	if (!sh_dirLight.LinkProgram())
	{
		return false;
	}

	sh_playtile.AttachShader(GL_VERTEX_SHADER, "Shaders/playTile.vert");
	sh_playtile.AttachShader(GL_FRAGMENT_SHADER, "Shaders/playTile.frag");

	sh_playtile.BindAttribLoc(0, "vs_in_pos");

	if (!sh_playtile.LinkProgram())
	{
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
}

int GameInstance::threadFunction(void *ptr)
{
	//int cnt;

	//for (cnt = 0; cnt < 200; ++cnt) {
	//	printf("\nThread counter: %d", cnt);
	//	SDL_Delay(50);
	//}

	GameInstance* pointr= static_cast<GameInstance *>(ptr);

	pointr->gameLogic.StartMatch(pointr->sea.getMyTiles(),pointr->sea.getEnemyTiles());

	std::cout << "The match is over." << std::endl;

	return 1;

}

void GameInstance::Update()
{
	// kameramozgatáshoz
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	cam_mainCamera.Update(delta_time);
	last_time = SDL_GetTicks();

}



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

	sh_dirLight.On(); //SHADER

	//float r = 2 * f_PI * SDL_GetTicks() / 1000.0f / 5.0f;
	glm::vec3 light_pos = glm::vec3(0, 0, 0);//glm::vec3(10 * cosf(r), 10, 10 * sinf(r));

	sh_dirLight.SetUniform("light_pos", light_pos);
	sh_dirLight.SetUniform("eye_pos", cam_mainCamera.GetEye());

	//m_program.SetUniform("use_texture", use_texture);
	//m_program.SetUniform("use_normal_map", use_normal_map);
	//m_program.SetUniform("use_light", use_light);

	//m_program.SetUniform("is_ambient", is_ambient);
	//m_program.SetUniform("is_diffuse", is_diffuse);
	//m_program.SetUniform("is_specular", is_specular);

	mountain.Draw(cam_mainCamera,sh_dirLight);
	terrain.Draw(cam_mainCamera,sh_dirLight);

	fleet.Draw(cam_mainCamera,sh_dirLight);

	sh_dirLight.Off();

	sh_playtile.On();

	sea.Draw(cam_mainCamera, sh_playtile);

	sh_playtile.Off();
}

void GameInstance::KeyboardDown(SDL_KeyboardEvent& key)
{
	cam_mainCamera.KeyboardDown(key);
	/*switch (key.keysym.sym) {
	case SDLK_q:
		is_filled = !is_filled;
		break;
	case SDLK_t:
		use_texture = !use_texture;
		break;
	case SDLK_l:
		use_light = !use_light;
		break;
	case SDLK_7:
		is_ambient = !is_ambient;
		break;
	case SDLK_8:
		is_diffuse = !is_diffuse;
		break;
	case SDLK_9:
		is_specular = !is_specular;
		break;
	case SDLK_n:
		use_normal_map = !use_normal_map;
		break;
	}*/
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

void GameInstance::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	cam_mainCamera.Resize(_w, _h);
}