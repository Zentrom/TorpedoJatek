#pragma once

//#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../Utils/gVertexBuffer.h"
#include "../../Utils/gShaderProgram.h"
#include "../../Utils/gCamera.h"

#include "../Globals.h"

class Ground
{
public:
	Ground(void);
	Ground(glm::vec3 terrainTranslate);
	~Ground(void);

	void Init();
	void Draw(gCamera &m_camera, gShaderProgram &m_program);

protected:
	gVertexBuffer	vb_ground;
	glm::vec3 ground_translate = glm::vec3(0, 0, 0)* GLOBALScale;
	float ground_rotate = 0.0f;
	glm::vec3 ground_rotate_angle = glm::vec3(0, 1.0f, 0);
	glm::vec3 ground_scale = glm::vec3(2.0f, 1.0f, 2.0f)* GLOBALScale;

};
