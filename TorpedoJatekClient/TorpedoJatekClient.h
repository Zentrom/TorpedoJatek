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
	// bels� elj�r�sok

	// g�mb parametrikus le�r�sa
	glm::vec3 GetUV(float u, float v);
	
	// a rajzol� met�dusaink
	void DrawGround();
	void DrawCone(); // a csonkak�pnak k�l�n van pal�stja...
	void DrawConeTop(); // ...�s teteje
	void DrawSuzanne();
	void DrawEarth();
	void DrawSun(glm::vec3 light_pos); // a "napot" mindig m�shova rajzoljuk; a param�ter a poz�ci�

	// Text�ra er�forr�s azonos�t�k
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

	gVertexBuffer	m_vb_ground; // a talaj geometri�ja
	gVertexBuffer	m_vb_cone; // a csonkakk�p pal�st geometri�ja
	gVertexBuffer	m_vb_cone_top; // a csonkak�p tetej�t alkot� k�rlap geometri�ja
	gVertexBuffer	m_vb_sphere; // egy egys�gsugar� g�mb geometri�ja

	Mesh			*m_mesh; // Suzanne

	int N = 20; // a g�mb felbont�sa
	int M = 20;

	// interakci�ra v�ltoztathat� be�ll�t�sok:

	bool is_filled = true; // kit�lt�tt vagy dr�tv�zas h�romsz�geket szeretn�nk?
	bool use_texture = true; // text�r�val vagy text�r�zatlanul rajzoljunk?
	bool use_normal_map = false; // haszn�ljunk-e normal mappinget?
	bool use_light = false; // megvil�g�t�ssal vagy an�lk�l?

	// melyik f�nykomponenst akarjuk bekapcsolni
	bool is_ambient = true;
	bool is_diffuse = true;
	bool is_specular = true;
};

