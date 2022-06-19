
#include "gCamera.h"

// Initializes a new instance of the gCamera class.
gCamera::gCamera(glm::vec3 pos) : camEye(pos), boundaryX(100.0f), boundaryY(100.0f), boundaryZ(100.0f),
camAt(0.0f), camUp(0.0f, 1.0f, 0.0f), speed(4.0f), goFw(0), goRight(0), fast(false)
{
	SetView(camEye * TorpedoGLOBAL::Scale,
		//camAt
		glm::normalize(camAt-camEye)
		, camUp);
	dist = glm::length(camAt - camEye);
	SetProj(45.0f, 800 / 600.0f, 0.001f, 1000.0f);
}

//gCamera::gCamera(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up) : m_speed(16.0f), m_goFw(0), m_goRight(0), m_dist(10), m_slow(false)
//{
//	SetView(_eye, _at, _up);
//}

gCamera::~gCamera(void)
{
}

//Beállítja a kamera határait
void gCamera::SetBoundaries(float bX, float bY, float bZ)
{
	boundaryX = bX;
	boundaryY = bY;
	boundaryZ = bZ;

	CameraResetCheck();
}

//Beállítja a nézését meg a járását
void gCamera::SetView(glm::vec3 eye, glm::vec3 at, glm::vec3 up)
{
	camEye = eye;
	camAt = at;
	camUp = up;

	camFw = glm::normalize(camAt - camEye);
	camSt = glm::normalize(glm::cross(camFw, camUp));

	dist = glm::length(camAt - camEye);

	camU = atan2f(camFw.z, camFw.x);
	camV = acosf(camFw.y);
}

void gCamera::SetProj(float angle, float aspect, float zn, float zf)
{
	matProj = glm::perspective(angle, aspect, zn, zf);
	matViewProj = matProj * viewMatrix;
}

//Gets the view matrix.
const glm::mat4& gCamera::GetViewMatrix() const
{
	return viewMatrix;
}

//Kamara mozgását kezeli
void gCamera::Update(float deltaTime)
{
	if (BoundaryCheckNextFrame(deltaTime)) {
		camEye += (goFw*camFw + goRight*camSt)*speed*deltaTime;
		camAt += (goFw*camFw + goRight*camSt)*speed*deltaTime;
	}

	viewMatrix = glm::lookAt(camEye, camAt, camUp);
	matViewProj = matProj * viewMatrix;
}

//Megnézi,hogy a következõ képkockára a kamera már kiesne-e a játéktérbõl
bool gCamera::BoundaryCheckNextFrame(float deltaTime)
{
	if (TorpedoGLOBAL::CameraBounds) {
		glm::vec3 tmpEye = camEye + (goFw*camFw + goRight*camSt)*speed*deltaTime;
		if (tmpEye.x > boundaryX || tmpEye.x < -boundaryX || tmpEye.y > boundaryY || tmpEye.y < -boundaryY / 8.0f
			|| tmpEye.z > boundaryZ || tmpEye.z < -boundaryZ)
		{
			return false;
		}
	}
	return true;
}

//Resets the camera if it's out of bounds
void gCamera::CameraResetCheck()
{
	if (TorpedoGLOBAL::CameraBounds) {
		if (camEye.x > boundaryX || camEye.x < -boundaryX || camEye.y > boundaryY || camEye.y < -boundaryY / 8.0f
			|| camEye.z > boundaryZ || camEye.z < -boundaryZ)
		{
			camEye = glm::vec3(-boundaryX / 2.0f, boundaryY / 2.0f, boundaryZ / 2.0f);
			SetView(camEye, camAt, camUp);
		}
	}
}

//Updates the UV
void gCamera::UpdateUV(float du, float dv)
{
	camU += du;
	camV = glm::clamp<float>(camV + dv, 0.1f, 3.1f);

	camAt = camEye + dist*glm::vec3(cosf(camU)*sinf(camV),
		cosf(camV),
		sinf(camU)*sinf(camV));

	camFw = glm::normalize(camAt - camEye);
	camSt = glm::normalize(glm::cross(camFw, camUp));
}

void gCamera::SetSpeed(float val)
{
	speed = val;
}

void gCamera::Resize(float w, float h, float fov, float viewDist)
{
	matProj = glm::perspective(fov, w / h, 0.01f, viewDist);

	matViewProj = matProj * viewMatrix;
}

void gCamera::KeyboardDown(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		if (!fast)
		{
			fast = true;
			speed *= 4.0f;
		}
		break;
	case SDLK_w:
		goFw = 1;
		break;
	case SDLK_s:
		goFw = -1;
		break;
	case SDLK_a:
		goRight = -1;
		break;
	case SDLK_d:
		goRight = 1;
		break;
	}
}

void gCamera::KeyboardUp(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		if (fast)
		{
			fast = false;
			speed /= 4.0f;
		}
		break;
	case SDLK_w:
	case SDLK_s:
		goFw = 0;
		break;
	case SDLK_a:
	case SDLK_d:
		goRight = 0;
		break;
	}
}

void gCamera::MouseMove(SDL_MouseMotionEvent& mouse)
{
	if (mouse.state & SDL_BUTTON_RMASK)
	{
		UpdateUV(mouse.xrel / 200.0f, mouse.yrel / 200.0f);
	}
}

void gCamera::LookAt(glm::vec3 at)
{
	SetView(camEye, at, camUp);
}

