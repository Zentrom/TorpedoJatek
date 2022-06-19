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
	//SeaTile();
	SeaTile(const glm::vec3 &trans);
	~SeaTile();

	void Init();
	void Draw(const gCamera& camera, gShaderProgram& sh_playtile) const;
	void setTranslate(const glm::vec3 &trans);

	static float getScaleXZ();
	const glm::vec3& getTranslate() const;

protected:

	gVertexBuffer vb_seatile; //grafikai modell adatok

	static const float scaleXZ; //tengermezõ skálázása XZ tengelyek mentén
	glm::vec3 seatileTranslate = glm::vec3(0);// , 0, 0) * TorpedoGLOBAL::Scale; //elmozgazás
	glm::vec3 seatileScale = glm::vec3(scaleXZ, 1.0f, scaleXZ) * TorpedoGLOBAL::Scale; //nagyítás

	glm::mat4 matWorld; //világ transzformáció 
	glm::mat4 matWorldIT; //VT inverze

	

};