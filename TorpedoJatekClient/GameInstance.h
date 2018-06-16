#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "Utils/gCamera.h"
#include "Utils/gShaderProgram.h"
#include "Utils/gVertexBuffer.h"
#include "Utils/Mesh_OGL3.h"

#include "Source/Frontend/Mountain.h"
#include "Source/Frontend/PlayTile.h"

class GameInstance
{
public:
	GameInstance(void);
	~GameInstance(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);
protected:
	enum GameState {GAME_WAITING};
	GameState GAME_STATE = GAME_WAITING;
	gCamera			m_camera;
	gShaderProgram	m_program;

	const float mountain_tile_offset = 4.0f + 1.0f; //tilescale fele
	const float mountaincenter_border_Xoffset = 6.0f / 2.0f; //scale2irányba megy
	Mountain mountain;
	PlayTile myPlayTiles[7*7];
	PlayTile enemyPlayTiles[7*7];

	/*bool is_filled = true;
	bool use_texture = true;
	bool use_normal_map = false;
	bool use_light = false;

	bool is_ambient = true;
	bool is_diffuse = true;
	bool is_specular = true;*/
};

