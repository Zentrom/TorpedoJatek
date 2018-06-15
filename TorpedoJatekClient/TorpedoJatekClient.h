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

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

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
	// belsõ eljárások

	// gömb parametrikus leírása
	glm::vec3 GetUV(float u, float v);
	
	// a rajzoló metódusaink
	void DrawGround();
	void DrawCone(); // a csonkakúpnak külön van palástja...
	void DrawConeTop(); // ...és teteje
	void DrawSuzanne();
	void DrawEarth();
	void DrawSun(glm::vec3 light_pos); // a "napot" mindig máshova rajzoljuk; a paraméter a pozíció

	// Textúra erõforrás azonosítók
	GLuint m_coneTextureID;
	GLuint m_coneNormalMapID;
	GLuint m_groundTextureID;
	GLuint m_groundNormalMapID;
	GLuint m_topTextureID;
	GLuint m_topNormalMapID;
	GLuint m_suzanneTextureID;
	GLuint m_suzanneNormalMapID;
	GLuint m_earthTextureID;
	GLuint m_earthNormalMapID;

	gCamera			m_camera;
	gShaderProgram	m_program;

	gVertexBuffer	m_vb_ground; // a talaj geometriája
	gVertexBuffer	m_vb_cone; // a csonkakkúp palást geometriája
	gVertexBuffer	m_vb_cone_top; // a csonkakúp tetejét alkotó körlap geometriája
	gVertexBuffer	m_vb_sphere; // egy egységsugarú gömb geometriája

	Mesh			*m_mesh; // Suzanne

	int N = 20; // a gömb felbontása
	int M = 20;

	// interakcióra változtatható beállítások:

	bool is_filled = true; // kitöltött vagy drótvázas háromszögeket szeretnénk?
	bool use_texture = true; // textúrával vagy textúrázatlanul rajzoljunk?
	bool use_normal_map = false; // használjunk-e normal mappinget?
	bool use_light = false; // megvilágítással vagy anélkül?

	// melyik fénykomponenst akarjuk bekapcsolni
	bool is_ambient = true;
	bool is_diffuse = true;
	bool is_specular = true;
};

