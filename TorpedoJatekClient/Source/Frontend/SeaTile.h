#pragma once

#include "../Globals.hpp"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Tenger mezõ
class SeaTile
{
public:
	SeaTile(void);
	~SeaTile(void);

	void Init();
	void Draw(gCamera &camera, gShaderProgram &sh_playtile);
	void setTranslate(const glm::vec3 &trans);

protected:

	gVertexBuffer	vb_seatile; //grafikai modell adatok

	glm::vec3 seatile_translate = glm::vec3(0, 0, 0)*TorpedoGLOBAL::Scale; //elmozgazás
	float seatile_rotate = 0.0f; //forgatás
	glm::vec3 seatile_rotate_angle = glm::vec3(0, 1.0f, 0); //forgatási tengely
	glm::vec3 seatile_scale = glm::vec3(2.0f, 1.0f, 2.0f)*TorpedoGLOBAL::Scale; //nagyítás

};