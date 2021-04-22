#pragma once

#include "../Globals.hpp"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Tenger mez�
class SeaTile
{
public:
	SeaTile(void);
	SeaTile(const glm::vec3 &trans);
	~SeaTile(void);

	void Init();
	void Draw(gCamera &camera, gShaderProgram &sh_playtile);
	void setTranslate(const glm::vec3 &trans);
	static float getScaleXZ();
	glm::vec3 getTranslate() const;

protected:

	gVertexBuffer	vb_seatile; //grafikai modell adatok
	static const float scaleXZ; //tengermez� sk�l�z�sa XZ tengelyek ment�n
	glm::vec3 seatile_translate = glm::vec3(0, 0, 0) * TorpedoGLOBAL::Scale; //elmozgaz�s
	float seatile_rotate = 0.0f; //forgat�s
	glm::vec3 seatile_rotate_angle = glm::vec3(0, 1.0f, 0); //forgat�si tengely
	glm::vec3 seatile_scale = glm::vec3(scaleXZ, 1.0f, scaleXZ) * TorpedoGLOBAL::Scale; //nagy�t�s

};