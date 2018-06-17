#pragma once

//#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../Utils/gVertexBuffer.h"
#include "../../Utils/gShaderProgram.h"
#include "../../Utils/gCamera.h"

class Ship
{
public:
	Ship(void);
	Ship(glm::vec3 battleShip);
	Ship(int tilePos);
	//Ship(int relativeBegin, int relativeEnd);
	~Ship(void);

	void Init();
	void Draw(gCamera &m_camera, gShaderProgram &m_program);

protected:
	gVertexBuffer	vb_ship;
	glm::vec3 ship_translate = glm::vec3(0, 0, 0);
	float ship_rotate = 0.0f;
	glm::vec3 ship_rotate_angle = glm::vec3(0, 1.0f, 0);
	glm::vec3 ship_scale = glm::vec3(1.25f, 1.25f, 1.25f);

};
