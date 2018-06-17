
//#include <GL/GLU.h>
#include <math.h>

//#include "Utils/GLUtils.hpp"
//#include "Utils/ObjParser_OGL3.h"

#include "GameInstance.h"



const float f_PI = 3.14159265359f;

GameInstance::GameInstance(void)
{
	
	/*
	m_coneTextureID = 0;
	m_coneNormalMapID = 0;
	m_groundTextureID = 0;
	m_groundNormalMapID = 0;
	m_topTextureID = 0;
	m_topNormalMapID = 0;
	m_suzanneTextureID = 0;
	m_suzanneNormalMapID = 0;
	m_earthTextureID = 0;
	m_earthNormalMapID = 0;

	m_mesh = 0;*/
}


GameInstance::~GameInstance(void)
{
}



bool GameInstance::Init()
{
	gameLogic.Init();
	actPlayTiles = gameLogic.getActiveTiles();

	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	//glEnable(GL_CULL_FACE);		// kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST);	// mélységi teszt bekapcsolása (takarás)

	mountain.Init();

	//tile
	float tile_transX = 0;
	float tile_transZ = 0;
	glm::vec3 tileResult = glm::vec3(0.0f);
	for (int i = 0; i < (7 * 7); i++) {
		tile_transX = (i % 7) * 2.0f;
		tile_transZ = ((i / 7) * 2.0f) - (3.0f*2.0f);

		tileResult += glm::vec3(mountaincenter_border_Xoffset,0,0) + 
			glm::vec3(mountain_tile_offset, 0, 0) +
			glm::vec3(tile_transX,0,tile_transZ);

		enemyPlayTiles[i] = PlayTile(tileResult);
		myPlayTiles[i] = PlayTile(glm::vec3(-1, 1, 1)*tileResult);

		enemyPlayTiles[i].Init();
		myPlayTiles[i].Init();
		tileResult = glm::vec3(0.0f);
	}

	//ships
	for (int i = 0; i < 16;i++) {
		tile_transX = ((actPlayTiles[i] % 10)-1) * 2.0f;
		tile_transZ = (((actPlayTiles[i] % 100)/10 -1) * 2.0f) - (3.0f*2.0f);

		tileResult += glm::vec3(mountaincenter_border_Xoffset, 0, 0) +
			glm::vec3(mountain_tile_offset, 0, 0) +
			glm::vec3(tile_transX, 0, tile_transZ);

		myShips[i] = Ship(glm::vec3(-1, 1, 1)*tileResult);


		//enemyShips[i].Init();
		myShips[i].Init();
		tileResult = glm::vec3(0.0f);
	}

	glm::vec3 battleShipOffset = glm::vec3(firstTile_battleShipOffset,0,0)+glm::vec3(mountaincenter_border_Xoffset+mountain_tile_offset, 0, 0);
	myBattleShip = Ship(-battleShipOffset);
	myBattleShip.Init();
	
	m_program.AttachShader(GL_VERTEX_SHADER, "Shaders/dirLight.vert");
	m_program.AttachShader(GL_FRAGMENT_SHADER, "Shaders/dirLight.frag");

	m_program.BindAttribLoc(0, "vs_in_pos");
	m_program.BindAttribLoc(1, "vs_in_color");
	//m_program.BindAttribLoc(1, "vs_in_normal");
	//m_program.BindAttribLoc(2, "vs_in_tex0");

	if (!m_program.LinkProgram())
	{
		return false;
	}

	sh_playtile.AttachShader(GL_VERTEX_SHADER, "Shaders/playTile.vert");
	sh_playtile.AttachShader(GL_FRAGMENT_SHADER, "Shaders/playTile.frag");

	sh_playtile.BindAttribLoc(0, "vs_in_pos");
	sh_playtile.BindAttribLoc(1, "vs_in_colorR");
	sh_playtile.BindAttribLoc(2, "vs_in_colorG");
	sh_playtile.BindAttribLoc(3, "vs_in_colorB");

	if (!sh_playtile.LinkProgram())
	{
		return false;
	}

	m_camera.SetProj(45.0f, 800.0f / 600.0f, 0.01f, 1000.0f);
	/*
	// textúrák betöltése
	m_coneTextureID = TextureFromFile("Resources/Textures/cylinder_texture.bmp");
	m_coneNormalMapID = TextureFromFile("Resources/Normals/cylinder_normal.bmp");
	m_topTextureID = TextureFromFile("Resources/Textures/top_texture.bmp");
	m_topNormalMapID = TextureFromFile("Resources/Normals/top_normal.bmp");
	m_groundTextureID = TextureFromFile("Resources/Textures/ground_texture.bmp");
	m_groundNormalMapID = TextureFromFile("Resources/Normals/ground_normal.bmp");
	m_suzanneTextureID = TextureFromFile("Resources/Textures/suzanne_texture.bmp");
	m_suzanneNormalMapID = TextureFromFile("Resources/Normals/suzanne_normal.bmp");
	m_earthTextureID = TextureFromFile("Resources/Textures/gomb_texture.bmp");
	m_earthNormalMapID = TextureFromFile("Resources/Normals/gomb_normal.bmp");

	// mesh betöltés
	m_mesh = ObjParser::parse("Resources/suzanne.obj");
	m_mesh->initBuffers();
	*/

	return true;
}

void GameInstance::Clean()
{
	/*glDeleteTextures(1, &m_coneTextureID);
	glDeleteTextures(1, &m_coneNormalMapID);
	glDeleteTextures(1, &m_groundTextureID);
	glDeleteTextures(1, &m_groundNormalMapID);
	glDeleteTextures(1, &m_topTextureID);
	glDeleteTextures(1, &m_topNormalMapID);
	glDeleteTextures(1, &m_suzanneTextureID);
	glDeleteTextures(1, &m_suzanneNormalMapID);
	glDeleteTextures(1, &m_earthTextureID);
	glDeleteTextures(1, &m_earthNormalMapID);*/

	m_program.Clean();
}

void GameInstance::Update()
{
	// kameramozgatáshoz
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	m_camera.Update(delta_time);
	last_time = SDL_GetTicks();
}



void GameInstance::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
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

	m_program.On(); // SHADER BEKAPCS

					// light_pos: y = 10 magasságban kering az XZ síkon, 5 mp alatt megtéve egy 10 sugarú körpályát
					// (ugyanez lesz majd a "nap" eltolása is az õ világtranszformációjában)
	//float r = 2 * f_PI * SDL_GetTicks() / 1000.0f / 5.0f;
	glm::vec3 light_pos = glm::vec3(0, 0, 0);//glm::vec3(10 * cosf(r), 10, 10 * sinf(r));

	// a shadernek átadjuk a rajzoláshoz szükséges paramétereket
	// hol a fényforrás és a kamera
	m_program.SetUniform("light_pos", light_pos);
	m_program.SetUniform("eye_pos", m_camera.GetEye());

	// használunk-e textúrát és fényt
	//m_program.SetUniform("use_texture", use_texture);
	//m_program.SetUniform("use_normal_map", use_normal_map);
	//m_program.SetUniform("use_light", use_light);

	// ha van fény, milyen komponenseket számoljunk
	//m_program.SetUniform("is_ambient", is_ambient);
	//m_program.SetUniform("is_diffuse", is_diffuse);
	//m_program.SetUniform("is_specular", is_specular);


	mountain.Draw(m_camera,m_program);

	for (int i = 0; i < 16; i++) {
		myShips[i].Draw(m_camera,m_program);
	}
	myBattleShip.Draw(m_camera, m_program);

	m_program.Off(); // SHADER KIKAPCS

	sh_playtile.On();
	for (int i = 0; i < (7 * 7); i++) {
		enemyPlayTiles[i].Draw(m_camera, sh_playtile);
		myPlayTiles[i].Draw(m_camera, sh_playtile);
	}
	sh_playtile.Off();
}

void GameInstance::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
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
	m_camera.KeyboardUp(key);
}

void GameInstance::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
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

	m_camera.Resize(_w, _h);
}