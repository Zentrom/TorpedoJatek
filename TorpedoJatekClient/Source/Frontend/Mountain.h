#pragma once

#include "../Globals.hpp"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//A hegy osztálya
class Mountain
{
public:
	Mountain(void);
	~Mountain(void);

	void Init();
	void Draw(gCamera &camera, gShaderProgram &sh_program);
	static float getWidth();
	static float getHeight();

protected:
	gVertexBuffer	vb_mountain; //grafikai adatok
	static const float heightY;
	static const float widthX; //Hegy szélessége X tengelyen
	const glm::vec3 mountain_translate = glm::vec3(0, 0, 0) * TorpedoGLOBAL::Scale; //mozgatás
	float mountain_rotate = 0.0f; //forgatás
	const glm::vec3 mountain_rotate_angle = glm::vec3(0, 1.0f, 0); //forgatás tengelye
	const glm::vec3 mountain_scale = glm::vec3(widthX, heightY, 20.0f) * TorpedoGLOBAL::Scale; //nagyítás

};