#pragma once

#include "../Source/Globals.hpp"

#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>

class gCamera
{
public:
	gCamera(const glm::vec3& pos, float mountain_height_y);
	~gCamera();

	void Update(float delta_time);

	void SetBoundaries(const glm::vec3& bound);
	void SetView(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	void SetProj(float angle, float aspect, float z_near, float z_far);
	void SetLookAt(const glm::vec3& at);

	const glm::vec3& GetEye() const { return camEye; }
	const glm::vec3& GetAt() const { return camAt; }
	const glm::vec3& GetUp() const { return camUp; }
	const glm::vec3& GetForwardDir() const { return camFw; }
	const glm::mat4& GetViewMatrix() const { return viewMatrix; }
	const glm::mat4& GetProj() const { return matProj; }
	const glm::mat4& GetViewProj() const { return matViewProj; }
	float getNear() const;
	float getFar() const;

	void Resize(int width, int height, float fov, float view_dist);

	void KeyboardDown(SDL_KeyboardEvent& key);
	void KeyboardUp(SDL_KeyboardEvent& key);
	void MouseMove(SDL_MouseMotionEvent& mouse);

private:
	bool BoundaryCheckNextFrame(float delta_time);
	void CameraResetCheck();
	void UpdateUV(float du, float dv);

	const float sensitivityDiv = 300.0f; //Minél nagyobb annál kissebb az egér sensitivity
	const float shiftSpeedUp = 4.0f; //Shift gombra mennyit gyorsulunk
	
	glm::mat4 viewMatrix; //Nézeti mátrix
	glm::mat4 matViewProj; //Projection*view
	glm::mat4 matProj = glm::perspective(45.0f, 800/600.0f, 0.001f, 100.0f); //Csak vetítés - Biztonsági alapérték
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f); //Felfelé mutató vektor
	glm::vec3 camFw; //Nézet irányába mutató egységvektor
	glm::vec3 camSt; //Jobbra mutató egységvektor

	glm::vec3 boundary = glm::vec3(100.0f, 100.0f, 100.0f); //Kameramozgás határai - Biztonsági alapérték
	float speed = 4.0f * TorpedoGLOBAL::Scale; //Kamera mozgás sebessége
	float goFw = 0; //Elõre megyünk-e
	float goRight = 0; //Jobbra megyünk-e
	bool fast = false; //Shift-gyorsítás
	float camU; //U gömbi koord - része az eye-ból való nézeti iránynak  
	float camV; //V gömbi koord - része az eye-ból való nézeti iránynak 
	float near = 0.001f; //Közeli vágósík
	float far = 200.0f; //Távoli vágósík

	glm::vec3 camEye; //Kamera pozíció
	glm::vec3 camAt; //Mely pontra néz a kamera
	const float mountainHeight; //Hegy magassága a boundaryhez
	
};

