#pragma once

#include "../Globals.hpp"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//A f�ld egy darabja
class Ground
{
public:
	Ground(void);
	Ground(const glm::vec3 &terrainTranslate);
	~Ground(void);

	void Init();
	void Draw(gCamera &camera, gShaderProgram &sh_program);

protected:
	gVertexBuffer	vb_ground; //grafikai adatokat t�rol� buffer
	glm::vec3 ground_translate = glm::vec3(0, 0, 0)* TorpedoGLOBAL::Scale; //elmozgat�s
	float ground_rotate = 0.0f; //forgat�s
	glm::vec3 ground_rotate_angle = glm::vec3(0, 1.0f, 0); //forgat�s tengelye
	glm::vec3 ground_scale = glm::vec3(2.0f, 1.0f, 2.0f)* TorpedoGLOBAL::Scale; //nagy�t�s

};
