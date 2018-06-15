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

// gömb parametrikus leírása: (u,v) -> (x,y,z)
glm::vec3 CMyApp::GetUV(float u, float v){
	u *= 2 * f_PI;
	v *= f_PI;
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	return glm::vec3(cu*sv, cv, su*sv);
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);		// kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST);	// mélységi teszt bekapcsolása (takarás)

	//
	// geometria letrehozasa
	//

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// TALAJ
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// attribútumok
	m_vb_ground.AddAttribute(0, 3);
	m_vb_ground.AddAttribute(1, 3);
	m_vb_ground.AddAttribute(2, 2);

	// a 0. attribútum (pozíciók)
	m_vb_ground.AddData(0, -10, 0, -10);
	m_vb_ground.AddData(0,  10, 0, -10);
	m_vb_ground.AddData(0, -10, 0,  10);
	m_vb_ground.AddData(0,  10, 0,  10);

	// az 1. attribútum (normálvektorok)
	m_vb_ground.AddData(1, 0, 1, 0);
	m_vb_ground.AddData(1, 0, 1, 0);
	m_vb_ground.AddData(1, 0, 1, 0);
	m_vb_ground.AddData(1, 0, 1, 0);

	// a 2. attribútum (textúrakoordináták)
	m_vb_ground.AddData(2, 0, 0);
	m_vb_ground.AddData(2, 1, 0);
	m_vb_ground.AddData(2, 0, 1);
	m_vb_ground.AddData(2, 1, 1);

	// Háromszögek indexelése
	m_vb_ground.AddIndex(1, 0, 2);
	m_vb_ground.AddIndex(1, 2, 3);

	// Inicializáljuk a puffereket (VAO, VBO, IB)
	m_vb_ground.InitBuffers();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// CSONKAKÚP PALÁST
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// a csonkakúp paraméterei: alsó körvonal sugara, felsö körvonal sugara, magasság
	// a csonkakúp az XZ síkon fog állni
	float r_low = 4;
	float r_high = 2;
	float h = 6;

	m_vb_cone.AddAttribute(0, 3);
	m_vb_cone.AddAttribute(1, 3);
	m_vb_cone.AddAttribute(2, 2);

	// 2*(N+1) vertexet definiálunk egy körív mentén:
	// felváltva rakunk egy vertexet lentre, egyet fentre
	for (int i = 0; i <= N; ++i){
		
		float a = i * 2 * f_PI / N; // szög

		// alsó vertexek y = 0 magasságban, r_low sugarú körír mentén,
		// felsõ vertexek y = h magasságban, r_high sugarú körív mentén
		// (a körívek origó középpontúak, XZ síkon fekszenek)
		m_vb_cone.AddData(0, r_low*cosf(a),  0,  r_low*sinf(a) );
		m_vb_cone.AddData(0, r_high*cosf(a), h, r_high*sinf(a));

		// kiszámoljuk a kúppalástot alkotó vertexek normálisát:
		// középponttól kifele mutatnak (X és Z), de Y irányban enyhén ferdén állnak,
		// a csonkakúp palást ferdeségétõl függõen
		glm::vec3 n = glm::normalize(glm::vec3(cosf(a), (r_low - r_high) / h, sinf(a)));
		m_vb_cone.AddData(1, n.x, n.y, n.z);
		m_vb_cone.AddData(1, n.x, n.y, n.z);

		// a textúrakoordináták: a textúrát sávokra bonta húzzúk rá a palástunkra
		m_vb_cone.AddData(2, 2*(float)i / N, 1);
		m_vb_cone.AddData(2, 2*(float)i / N, 0);
	}

	// vertex indexek a háromszögekhez
	// palást = N darab négyzet, egy négyzet = két háromszög
	for (int i = 0; i < N; ++i){
		m_vb_cone.AddIndex(2 * i,     2 * i + 1, 2 * i + 2);
		m_vb_cone.AddIndex(2 * i + 1, 2 * i + 3, 2 * i + 2);
	}

	m_vb_cone.InitBuffers();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// CSONKAKÚP TETEJE: KÖRLAP
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	m_vb_cone_top.AddAttribute(0, 3);
	m_vb_cone_top.AddAttribute(1, 3);
	m_vb_cone_top.AddAttribute(2, 2);

	// egy vertex középre, y = h magasságba
	m_vb_cone_top.AddData(0, 0, h, 0);
	m_vb_cone_top.AddData(1, 0, 1, 0);
	m_vb_cone_top.AddData(2, 0.5, 0.5);

	// N+1 vertex a a körvonalon
	for (int i = 0; i <= N; ++i){

		float a = i * 2 * f_PI / N;
		// y = h magasságban, r_high sugarú körvonal mentén
		m_vb_cone_top.AddData(0, r_high * cosf(a), h, r_high * sinf(a));
		// a normálvektorok minden vextex esetén felfelé mutatnak
		m_vb_cone_top.AddData(1, 0, 1, 0);
		// a textúra közepébõl kivágunk egy 0.5 sugarú körlapot
		m_vb_cone_top.AddData(2, 0.5 * cosf(a) + 0.5, 0.5 * sinf(a) + 0.5);
	}
	// indexek a körlap megalkotásához (N darab háromszög)
	for (int i = 0; i < N; ++i){
		m_vb_cone_top.AddIndex(0, i + 2, i + 1);
	}

	m_vb_cone_top.InitBuffers();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// GÖMB
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	m_vb_sphere.AddAttribute(0, 3);
	m_vb_sphere.AddAttribute(1, 3);
	m_vb_sphere.AddAttribute(2, 2);

	// N*M-es rács bejárása, a vertexek (x,y,z) koordinátáját a GetUV függvénnyel kapjuk
	for (int i = 0; i <= N; ++i){
		for (int j = 0; j <= M; ++j){
			float u = i / (float)N;
			float v = j / (float)M;
			// pozíció: gömbi koordináták
			m_vb_sphere.AddData(0, GetUV(u, v));
			// szerencsékre gömb esetén ugyanaz a normálvektor:
			// az origóból a gömbfelszín irányába mutató irányvektor
			// de ez csak a gömbnél van így! (Lásd fentebb a csonkakúp palást, ott másképp kellett számolnunk!)
			m_vb_sphere.AddData(1, GetUV(u, v));
			// a textúra-koordináták megyeznek az (u,v) pont koordinátájával
			// (kivéve, hogy a kép rossz irányba állna)
			m_vb_sphere.AddData(2, 1-u, v);
		}
	}
	// az indexek meghatározása minden négyzethez (N*M négyzetünk van)
	for (int i = 0; i < N; ++i){
		for (int j = 0; j < M; ++j){
			// az egyik háromszög a négyzet egyik fele
			m_vb_sphere.AddIndex((i)		+ (j)*		(N + 1),
								 (i)		+ (j + 1) *	(N + 1),
								 (i + 1)	+ (j) *		(N + 1)
							 );
			// a másik háromszög a négyzet másik fele
			m_vb_sphere.AddIndex((i + 1)	+ (j) *		(N + 1),
								 (i)		+ (j + 1) *	(N + 1),
								 (i + 1)	+ (j + 1) *	(N + 1)
							 );
		}
	}

	m_vb_sphere.InitBuffers();

	//
	// shaderek betöltése
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
	// egyéb inicializálás
	//

	m_camera.SetProj(45.0f, 640.0f/480.0f, 0.01f, 1000.0f);

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
	// kameramozgatáshoz
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time)/1000.0f;
	m_camera.Update(delta_time);
	last_time = SDL_GetTicks();
}

// talaj kirajzolása
void CMyApp::DrawGround(){

	glm::mat4 matWorld = glm::mat4(1.0f); // nem változtatunk rajta
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

// csonkakúp palást kirajzolása
void CMyApp::DrawCone(){

	glm::mat4 matWorld = glm::mat4(1.0f); // nem mozdítjuk el
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

// csonkakúp tetejének kirajzolása
void CMyApp::DrawConeTop(){

	glm::mat4 matWorld = glm::mat4(1.0f); // nem mozdítjuk el
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

	// kétszeresére nagyítjuk és eltoljuk
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

	// háromszorosára nagyítjuk és eltoljuk
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

	// a napot jelképezõ gömböt (lekicsinyítve) oda helyezzük, ahol a fényforrásunk is van
	glm::mat4 matWorld = glm::translate<float>(light_pos) * glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mvp = m_camera.GetViewProj() *matWorld;
	m_program.SetUniform("MVP", mvp);

	m_program.SetUniform("is_sun", true); // értesítjük a shadert, hogy a most kirajzolandó objektum a "nap":
	// rá ne hasson a megvilágítás, hanem legyen mindig konstans sárga (lásd a fregmens shader kódját)
	m_vb_sphere.On();
	m_vb_sphere.DrawIndexed(GL_TRIANGLES, 0, 2 * 3 * N * M, 0);
	m_vb_sphere.Off();
	m_program.SetUniform("is_sun", false); // ne felejtsük el visszaállítani...

}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// megjelenítés módja
	if (is_filled){
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	m_program.On(); // SHADER BEKAPCS

	// light_pos: y = 10 magasságban kering az XZ síkon, 5 mp alatt megtéve egy 10 sugarú körpályát
	// (ugyanez lesz majd a "nap" eltolása is az õ világtranszformációjában)
	float r = 2 * f_PI * SDL_GetTicks() / 1000.0f / 5.0f;
	glm::vec3 light_pos = glm::vec3(10 * cosf(r), 10, 10 * sinf(r));
	
	// a shadernek átadjuk a rajzoláshoz szükséges paramétereket
	// hol a fényforrás és a kamera
	m_program.SetUniform("light_pos", light_pos);
	m_program.SetUniform("eye_pos", m_camera.GetEye());

	// használunk-e textúrát és fényt
	m_program.SetUniform("use_texture", use_texture);
	m_program.SetUniform("use_normal_map", use_normal_map);
	m_program.SetUniform("use_light", use_light);

	// ha van fény, milyen komponenseket számoljunk
	m_program.SetUniform("is_ambient", is_ambient);
	m_program.SetUniform("is_diffuse", is_diffuse);
	m_program.SetUniform("is_specular", is_specular);

	// az egyes objektumok kirajzolása
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

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
}