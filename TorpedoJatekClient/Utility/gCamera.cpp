
#include "gCamera.h"

gCamera::gCamera(const glm::vec3& pos, float mountain_height_y) : camEye(pos), camAt(0.0f),
	mountainHeight(mountain_height_y)
{
	SetView(camEye * TorpedoGLOBAL::Scale, camAt, camUp);
}

gCamera::~gCamera()
{
}

//Beállítja a kamera határait
void gCamera::SetBoundaries(const glm::vec3& bound)
{
	boundary = bound;

	CameraResetCheck();
}

//Beállítja a kamera nézetét
void gCamera::SetView(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	camEye = eye;
	camAt = at;
	camUp = up;

	camFw = glm::normalize(camAt - camEye);
	camSt = glm::normalize(glm::cross(camFw, camUp));

	camU = atan2f(camFw.z, camFw.x);
	camV = acosf(camFw.y);
	viewMatrix = glm::lookAt(camEye, camAt, camUp);
}

//Kiszámítja a nézet-projekciós mátrixot
void gCamera::SetProj(float angle, float aspect, float z_near, float z_far)
{
	near = z_near;
	far = z_far;

	matProj = glm::perspective(angle, aspect, z_near, z_far);
	matViewProj = matProj * viewMatrix;
}

//Kamara mozgását kezeli
void gCamera::Update(float delta_time)
{
	if (BoundaryCheckNextFrame(delta_time)) {
		camEye += (goFw*camFw + goRight*camSt)*speed*delta_time;
		camAt += (goFw*camFw + goRight*camSt)*speed*delta_time;

		viewMatrix = glm::lookAt(camEye, camAt, camUp);
		matViewProj = matProj * viewMatrix;
	}
}

//Megnézi,hogy a következõ képkockára a kamera már kiesne-e a játéktérbõl
bool gCamera::BoundaryCheckNextFrame(float delta_time)
{
	if (TorpedoGLOBAL::CameraBounds) {
		glm::vec3 tmpEye = camEye + (goFw*camFw + goRight*camSt)*speed*delta_time;
		if (tmpEye.x > boundary.x || tmpEye.x < -boundary.x || tmpEye.y > boundary.y || tmpEye.y < -boundary.y / mountainHeight
			|| tmpEye.z > boundary.z || tmpEye.z < -boundary.z)
		{
			return false;
		}
	}
	return true;
}

//Visszaállítja a kamera pozíciót ha az a határokon kívûl esik
void gCamera::CameraResetCheck()
{
	if (TorpedoGLOBAL::CameraBounds) {
		if (camEye.x > boundary.x || camEye.x < -boundary.x || camEye.y > boundary.y || camEye.y < -boundary.y / mountainHeight
			|| camEye.z > boundary.z || camEye.z < -boundary.z)
		{
			camEye = glm::vec3(-boundary.x / 2.0f, boundary.y / 2.0f, boundary.z / 2.0f);
			SetView(camEye, camAt, camUp);
		}
	}
}

//UV gömbi koord alapján kezeli a kamera forgatást
void gCamera::UpdateUV(float du, float dv)
{
	camU += du;
	camV = glm::clamp<float>(camV + dv, 0.1f, 3.1f);

	camAt = camEye + glm::vec3(cosf(camU)*sinf(camV), cosf(camV),
		sinf(camU)*sinf(camV));

	camFw = glm::normalize(camAt - camEye);
	camSt = glm::normalize(glm::cross(camFw, camUp));
}

//Ablak átméretezés esetén
void gCamera::Resize(int width, int height, float fov, float view_dist)
{
	matProj = glm::perspective(fov, width / static_cast<float>(height), 0.001f, view_dist);

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
			speed *= shiftSpeedUp;
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
			speed /= shiftSpeedUp;
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
		UpdateUV(mouse.xrel / sensitivityDiv, mouse.yrel / sensitivityDiv);
	}
}

//Nem a GLM-es!
void gCamera::SetLookAt(const glm::vec3& at)
{
	SetView(camEye, at, camUp);
}

float gCamera::getNear() const
{
	return near;
}

float gCamera::getFar() const
{
	return far;
}