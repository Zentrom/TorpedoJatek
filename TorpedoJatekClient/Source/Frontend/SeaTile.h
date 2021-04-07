#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include "../Globals.h"

class SeaTile
{
public:
	SeaTile(void);
	SeaTile(glm::vec3 translate);
	~SeaTile(void);

	void Init();
	void Draw(gCamera &camera, gShaderProgram &sh_playtile);
	void setTranslate(glm::vec3 trans);

protected:

	gVertexBuffer	vb_seatile;

	glm::vec3 seatile_translate = glm::vec3(0, 0, 0)*TorpedoGLOBAL::Scale;
	float seatile_rotate = 0.0f;
	glm::vec3 seatile_rotate_angle = glm::vec3(0, 1.0f, 0);
	glm::vec3 seatile_scale = glm::vec3(2.0f, 1.0f, 2.0f)*TorpedoGLOBAL::Scale;

};