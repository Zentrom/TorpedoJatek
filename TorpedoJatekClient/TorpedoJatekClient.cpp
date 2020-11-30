#include "TorpedoJatekClient.h"
#include "Utils/GLUtils.hpp"

#include <GL/GLU.h>
#include <math.h>

#include "Utils/ObjParser_OGL3.h"

const float f_PI = 3.14159265359f;

CMyApp::CMyApp(void)
{
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

	m_mesh = 0;
}


CMyApp::~CMyApp(void)
{
}

// g�mb parametrikus le�r�sa: (u,v) -> (x,y,z)
glm::vec3 CMyApp::GetUV(float u, float v){
	u *= 2 * f_PI;
	v *= f_PI;
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	return glm::vec3(cu*sv, cv, su*sv);
}

bool CMyApp::Init()
{
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);		// kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST);	// m�lys�gi teszt bekapcsol�sa (takar�s)

	//
	// geometria letrehozasa
	//

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// TALAJ
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// attrib�tumok
	m_vb_ground.AddAttribute(0, 3);
	m_vb_ground.AddAttribute(1, 3);
	m_vb_ground.AddAttribute(2, 2);

	// a 0. attrib�tum (poz�ci�k)
	m_vb_ground.AddData(0, -10, 0, -10);
	m_vb_ground.AddData(0,  10, 0, -10);
	m_vb_ground.AddData(0, -10, 0,  10);
	m_vb_ground.AddData(0,  10, 0,  10);

	// az 1. attrib�tum (norm�lvektorok)
	m_vb_ground.AddData(1, 0, 1, 0);
	m_vb_ground.AddData(1, 0, 1, 0);
	m_vb_ground.AddData(1, 0, 1, 0);
	m_vb_ground.AddData(1, 0, 1, 0);

	// a 2. attrib�tum (text�rakoordin�t�k)
	m_vb_ground.AddData(2, 0, 0);
	m_vb_ground.AddData(2, 1, 0);
	m_vb_ground.AddData(2, 0, 1);
	m_vb_ground.AddData(2, 1, 1);

	// H�romsz�gek indexel�se
	m_vb_ground.AddIndex(1, 0, 2);
	m_vb_ground.AddIndex(1, 2, 3);

	// Inicializ�ljuk a puffereket (VAO, VBO, IB)
	m_vb_ground.InitBuffers();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// CSONKAK�P PAL�ST
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// a csonkak�p param�terei: als� k�rvonal sugara, fels� k�rvonal sugara, magass�g
	// a csonkak�p az XZ s�kon fog �llni
	float r_low = 4;
	float r_high = 2;
	float h = 6;

	m_vb_cone.AddAttribute(0, 3);
	m_vb_cone.AddAttribute(1, 3);
	m_vb_cone.AddAttribute(2, 2);

	// 2*(N+1) vertexet defini�lunk egy k�r�v ment�n:
	// felv�ltva rakunk egy vertexet lentre, egyet fentre
	for (int i = 0; i <= N; ++i){
		
		float a = i * 2 * f_PI / N; // sz�g

		// als� vertexek y = 0 magass�gban, r_low sugar� k�r�r ment�n,
		// fels� vertexek y = h magass�gban, r_high sugar� k�r�v ment�n
		// (a k�r�vek orig� k�z�ppont�ak, XZ s�kon fekszenek)
		m_vb_cone.AddData(0, r_low*cosf(a),  0,  r_low*sinf(a) );
		m_vb_cone.AddData(0, r_high*cosf(a), h, r_high*sinf(a));

		// kisz�moljuk a k�ppal�stot alkot� vertexek norm�lis�t:
		// k�z�ppontt�l kifele mutatnak (X �s Z), de Y ir�nyban enyh�n ferd�n �llnak,
		// a csonkak�p pal�st ferdes�g�t�l f�gg�en
		glm::vec3 n = glm::normalize(glm::vec3(cosf(a), (r_low - r_high) / h, sinf(a)));
		m_vb_cone.AddData(1, n.x, n.y, n.z);
		m_vb_cone.AddData(1, n.x, n.y, n.z);

		// a text�rakoordin�t�k: a text�r�t s�vokra bonta h�zz�k r� a pal�stunkra
		m_vb_cone.AddData(2, 2*(float)i / N, 1);
		m_vb_cone.AddData(2, 2*(float)i / N, 0);
	}

	// vertex indexek a h�romsz�gekhez
	// pal�st = N darab n�gyzet, egy n�gyzet = k�t h�romsz�g
	for (int i = 0; i < N; ++i){
		m_vb_cone.AddIndex(2 * i,     2 * i + 1, 2 * i + 2);
		m_vb_cone.AddIndex(2 * i + 1, 2 * i + 3, 2 * i + 2);
	}

	m_vb_cone.InitBuffers();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// CSONKAK�P TETEJE: K�RLAP
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	m_vb_cone_top.AddAttribute(0, 3);
	m_vb_cone_top.AddAttribute(1, 3);
	m_vb_cone_top.AddAttribute(2, 2);

	// egy vertex k�z�pre, y = h magass�gba
	m_vb_cone_top.AddData(0, 0, h, 0);
	m_vb_cone_top.AddData(1, 0, 1, 0);
	m_vb_cone_top.AddData(2, 0.5, 0.5);

	// N+1 vertex a a k�rvonalon
	for (int i = 0; i <= N; ++i){

		float a = i * 2 * f_PI / N;
		// y = h magass�gban, r_high sugar� k�rvonal ment�n
		m_vb_cone_top.AddData(0, r_high * cosf(a), h, r_high * sinf(a));
		// a norm�lvektorok minden vextex eset�n felfel� mutatnak
		m_vb_cone_top.AddData(1, 0, 1, 0);
		// a text�ra k�zep�b�l kiv�gunk egy 0.5 sugar� k�rlapot
		m_vb_cone_top.AddData(2, 0.5 * cosf(a) + 0.5, 0.5 * sinf(a) + 0.5);
	}
	// indexek a k�rlap megalkot�s�hoz (N darab h�romsz�g)
	for (int i = 0; i < N; ++i){
		m_vb_cone_top.AddIndex(0, i + 2, i + 1);
	}

	m_vb_cone_top.InitBuffers();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// G�MB
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	m_vb_sphere.AddAttribute(0, 3);
	m_vb_sphere.AddAttribute(1, 3);
	m_vb_sphere.AddAttribute(2, 2);

	// N*M-es r�cs bej�r�sa, a vertexek (x,y,z) koordin�t�j�t a GetUV f�ggv�nnyel kapjuk
	for (int i = 0; i <= N; ++i){
		for (int j = 0; j <= M; ++j){
			float u = i / (float)N;
			float v = j / (float)M;
			// poz�ci�: g�mbi koordin�t�k
			m_vb_sphere.AddData(0, GetUV(u, v));
			// szerencs�kre g�mb eset�n ugyanaz a norm�lvektor:
			// az orig�b�l a g�mbfelsz�n ir�ny�ba mutat� ir�nyvektor
			// de ez csak a g�mbn�l van �gy! (L�sd fentebb a csonkak�p pal�st, ott m�sk�pp kellett sz�molnunk!)
			m_vb_sphere.AddData(1, GetUV(u, v));
			// a text�ra-koordin�t�k megyeznek az (u,v) pont koordin�t�j�val
			// (kiv�ve, hogy a k�p rossz ir�nyba �llna)
			m_vb_sphere.AddData(2, 1-u, v);
		}
	}
	// az indexek meghat�roz�sa minden n�gyzethez (N*M n�gyzet�nk van)
	for (int i = 0; i < N; ++i){
		for (int j = 0; j < M; ++j){
			// az egyik h�romsz�g a n�gyzet egyik fele
			m_vb_sphere.AddIndex((i)		+ (j)*		(N + 1),
								 (i)		+ (j + 1) *	(N + 1),
								 (i + 1)	+ (j) *		(N + 1)
							 );
			// a m�sik h�romsz�g a n�gyzet m�sik fele
			m_vb_sphere.AddIndex((i + 1)	+ (j) *		(N + 1),
								 (i)		+ (j + 1) *	(N + 1),
								 (i + 1)	+ (j + 1) *	(N + 1)
							 );
		}
	}

	m_vb_sphere.InitBuffers();

	//
	// shaderek bet�lt�se
	//
	m_program.AttachShader(GL_VERTEX_SHADER, "Shaders/dirLight.vert");
	m_program.AttachShader(GL_FRAGMENT_SHADER, "Shaders/dirLight.frag");

	m_program.BindAttribLoc(0, "vs_in_pos");
	m_program.BindAttribLoc(1, "vs_in_normal");
	m_program.BindAttribLoc(2, "vs_in_tex0");

	if ( !m_program.LinkProgram() )
	{
		return false;
	}

	//
	// egy�b inicializ�l�s
	//

	m_camera.SetProj(45.0f, 640.0f/480.0f, 0.01f, 1000.0f);

	// text�r�k bet�lt�se
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

	// mesh bet�lt�s
	m_mesh = ObjParser::parse("Resources/suzanne.obj");
	m_mesh->initBuffers();

	return true;
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_coneTextureID);
	glDeleteTextures(1, &m_coneNormalMapID);
	glDeleteTextures(1, &m_groundTextureID);
	glDeleteTextures(1, &m_groundNormalMapID);
	glDeleteTextures(1, &m_topTextureID);
	glDeleteTextures(1, &m_topNormalMapID);
	glDeleteTextures(1, &m_suzanneTextureID);
	glDeleteTextures(1, &m_suzanneNormalMapID);
	glDeleteTextures(1, &m_earthTextureID);
	glDeleteTextures(1, &m_earthNormalMapID);

	m_program.Clean();
}

void CMyApp::Update()
{
	// kameramozgat�shoz
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time)/1000.0f;
	m_camera.Update(delta_time);
	last_time = SDL_GetTicks();
}

// talaj kirajzol�sa
void CMyApp::DrawGround(){

	glm::mat4 matWorld = glm::mat4(1.0f); // nem v�ltoztatunk rajta
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = m_camera.GetViewProj() *matWorld;

	m_program.SetUniform("world", matWorld);
	m_program.SetUniform("worldIT", matWorldIT);
	m_program.SetUniform("MVP", mvp);

	m_program.SetTexture("texImage", 0, m_groundTextureID);
	m_program.SetTexture("texNormal", 1, m_groundNormalMapID);

	m_vb_ground.On();
	m_vb_ground.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	m_vb_ground.Off();

}

// csonkak�p pal�st kirajzol�sa
void CMyApp::DrawCone(){

	glm::mat4 matWorld = glm::mat4(1.0f); // nem mozd�tjuk el
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = m_camera.GetViewProj() *matWorld;

	m_program.SetUniform("world", matWorld);
	m_program.SetUniform("worldIT", matWorldIT);
	m_program.SetUniform("MVP", mvp);

	m_program.SetTexture("texImage", 0, m_coneTextureID);
	m_program.SetTexture("texNormal", 1, m_coneNormalMapID);

	m_vb_cone.On();
	m_vb_cone.DrawIndexed(GL_TRIANGLES, 0, 2 * 3 * N, 0);
	m_vb_cone.Off();

}

// csonkak�p tetej�nek kirajzol�sa
void CMyApp::DrawConeTop(){

	glm::mat4 matWorld = glm::mat4(1.0f); // nem mozd�tjuk el
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = m_camera.GetViewProj() *matWorld;

	m_program.SetUniform("world", matWorld);
	m_program.SetUniform("worldIT", matWorldIT);
	m_program.SetUniform("MVP", mvp);

	m_program.SetTexture("texImage", 0, m_topTextureID);
	m_program.SetTexture("texNormal", 1, m_topNormalMapID);

	m_vb_cone_top.On();
	m_vb_cone_top.DrawIndexed(GL_TRIANGLES, 0, 3 * N, 0);
	m_vb_cone_top.Off();

}

void CMyApp::DrawSuzanne(){

	// k�tszeres�re nagy�tjuk �s eltoljuk
	glm::mat4 matWorld = glm::translate(glm::vec3(5.0f, 5.0f, 0.0f)) * glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = m_camera.GetViewProj() *matWorld;

	m_program.SetUniform("world", matWorld);
	m_program.SetUniform("worldIT", matWorldIT);
	m_program.SetUniform("MVP", mvp);

	m_program.SetTexture("texImage", 0, m_suzanneTextureID);
	m_program.SetTexture("texNormal", 1, m_suzanneNormalMapID);

	m_mesh->draw();

}

void CMyApp::DrawEarth(){

	// h�romszoros�ra nagy�tjuk �s eltoljuk
	glm::mat4 matWorld = glm::translate(glm::vec3(-6.0f, 6.0f, 0.0f)) * glm::scale(glm::vec3(3.0f, 3.0f, 3.0f));
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = m_camera.GetViewProj() *matWorld;

	m_program.SetUniform("world", matWorld);
	m_program.SetUniform("worldIT", matWorldIT);
	m_program.SetUniform("MVP", mvp);

	m_program.SetTexture("texImage", 0, m_earthTextureID);
	m_program.SetTexture("texNormal", 1, m_earthNormalMapID);

	m_vb_sphere.On();
	m_vb_sphere.DrawIndexed(GL_TRIANGLES, 0, 2 * 3 * N * M, 0);
	m_vb_sphere.Off();

}

void CMyApp::DrawSun(glm::vec3 light_pos){

	// a napot jelk�pez� g�mb�t (lekicsiny�tve) oda helyezz�k, ahol a f�nyforr�sunk is van
	glm::mat4 matWorld = glm::translate<float>(light_pos) * glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mvp = m_camera.GetViewProj() *matWorld;
	m_program.SetUniform("MVP", mvp);

	m_program.SetUniform("is_sun", true); // �rtes�tj�k a shadert, hogy a most kirajzoland� objektum a "nap":
	// r� ne hasson a megvil�g�t�s, hanem legyen mindig konstans s�rga (l�sd a fregmens shader k�dj�t)
	m_vb_sphere.On();
	m_vb_sphere.DrawIndexed(GL_TRIANGLES, 0, 2 * 3 * N * M, 0);
	m_vb_sphere.Off();
	m_program.SetUniform("is_sun", false); // ne felejts�k el vissza�ll�tani...

}

void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// megjelen�t�s m�dja
	if (is_filled){
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	m_program.On(); // SHADER BEKAPCS

	// light_pos: y = 10 magass�gban kering az XZ s�kon, 5 mp alatt megt�ve egy 10 sugar� k�rp�ly�t
	// (ugyanez lesz majd a "nap" eltol�sa is az � vil�gtranszform�ci�j�ban)
	float r = 2 * f_PI * SDL_GetTicks() / 1000.0f / 5.0f;
	glm::vec3 light_pos = glm::vec3(10 * cosf(r), 10, 10 * sinf(r));
	
	// a shadernek �tadjuk a rajzol�shoz sz�ks�ges param�tereket
	// hol a f�nyforr�s �s a kamera
	m_program.SetUniform("light_pos", light_pos);
	m_program.SetUniform("eye_pos", m_camera.GetEye());

	// haszn�lunk-e text�r�t �s f�nyt
	m_program.SetUniform("use_texture", use_texture);
	m_program.SetUniform("use_normal_map", use_normal_map);
	m_program.SetUniform("use_light", use_light);

	// ha van f�ny, milyen komponenseket sz�moljunk
	m_program.SetUniform("is_ambient", is_ambient);
	m_program.SetUniform("is_diffuse", is_diffuse);
	m_program.SetUniform("is_specular", is_specular);

	// az egyes objektumok kirajzol�sa
	DrawSun(light_pos);
	DrawGround();
	DrawCone();
	DrawConeTop();
	DrawSuzanne();
	DrawEarth();

	m_program.Off(); // SHADER KIKAPCS

}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
	switch (key.keysym.sym){
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
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
}