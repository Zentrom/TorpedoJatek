#pragma once

//#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include "../Globals.h"

class Ship
{
public:
	Ship(void);
	Ship(glm::vec3 fleetTranslate);
	Ship(int tilePos);
	//Ship(int relativeBegin, int relativeEnd);
	~Ship(void);

	void Init(bool isEnemy);
	void Draw(gCamera &camera, gShaderProgram &sh_program);

protected:
	gVertexBuffer	vb_ship;
	glm::vec3 ship_translate = glm::vec3(0, 0, 0)* TorpedoGLOBAL::Scale;
	float ship_rotate = 0.0f;
	glm::vec3 ship_rotate_angle = glm::vec3(0, 1.0f, 0);
	glm::vec3 ship_scale = glm::vec3(1.25f, 1.25f, 1.25f) * TorpedoGLOBAL::Scale;

};
