#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include "../Globals.h"

//A hegy oszt�lya
class Mountain
{
public:
	Mountain(void);
	~Mountain(void);

	void Init();
	void Draw(gCamera &camera, gShaderProgram &sh_program);

protected:
	gVertexBuffer	vb_mountain; //grafikai adatok
	glm::vec3 mountain_translate = glm::vec3(0, 0, 0) * TorpedoGLOBAL::Scale; //mozgat�s
	float mountain_rotate = 0.0f; //forgat�s
	glm::vec3 mountain_rotate_angle = glm::vec3(0, 1.0f, 0); //forgat�s tengelye
	glm::vec3 mountain_scale = glm::vec3(6.0f,6.0f,20.0f)* TorpedoGLOBAL::Scale; //nagy�t�s

};
