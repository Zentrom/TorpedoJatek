#pragma once

#include "../Source/Globals.hpp"

#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <math.h>

class gCamera
{
public:
	gCamera(glm::vec3 pos);
	//gCamera(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up);
	~gCamera(void);

	void Update(float deltaTime);

	void SetBoundaries(float bX, float bY, float bZ);
	void SetView(glm::vec3 eye, glm::vec3 at, glm::vec3 up);
	void SetProj(float angle, float aspect, float zn, float zf);
	void LookAt(glm::vec3 at);
	void SetSpeed(float val);
	glm::mat4 GetViewMatrix();
	glm::vec3& GetEye()
	{
		return camEye;
	}

	glm::vec3 GetAt()
	{
		return camAt;
	}

	glm::vec3 GetUp()
	{
		return camUp;
	}

	glm::mat4 GetProj()
	{
		return matProj;
	}

	glm::mat4 GetViewProj()
	{
		return matViewProj;
	}

	void Resize(float w, float h, float fov, float viewDist);

	void KeyboardDown(SDL_KeyboardEvent& key);
	void KeyboardUp(SDL_KeyboardEvent& key);
	void MouseMove(SDL_MouseMotionEvent& mouse);

private:
	bool BoundaryCheckNextFrame(float deltaTime);
	void CameraResetCheck();
	void UpdateUV(float du, float dv);

	//Boundaries the camera cannot pass
	float boundaryX;
	float boundaryY;
	float boundaryZ;

	// The traversal speed of the camera
	float speed;
	// The view matrix of the camera
	glm::mat4 viewMatrix;
	glm::mat4 matViewProj;

	bool fast;

	// The camera position.
	glm::vec3 camEye;
	// The vector pointing upwards
	glm::vec3 camUp;
	// The camera look at point.
	glm::vec3 camAt;

	// The u spherical coordinate of the spherical coordinate pair (u,v) denoting the
	// current viewing direction from the view position m_eye. 
	float camU;

	// The v spherical coordinate of the spherical coordinate pair (u,v) denoting the
	// current viewing direction from the view position m_eye. 
	float camV;

	// The distance of the look at point from the camera. 
	float dist;

	// The unit vector pointing towards the viewing direction.
	glm::vec3 camFw;
	// The unit vector pointing to the 'right'
	glm::vec3 camSt;

	glm::mat4 matProj;

	float goFw;
	float goRight;
};

