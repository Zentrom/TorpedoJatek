#pragma once

#include "../Globals.hpp"
#include "BShipProjectile.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"
#include "../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Csatahajó ágyúja
class BShipCannon
{
public:
	BShipCannon(void);
	BShipCannon(const glm::vec3 &battleShipTranslate,bool ally, float battleShipScaleX);
	~BShipCannon(void);

	void Draw(gCamera& camera, gShaderProgram& sh_program, glm::mat4 sharedtrans);
	BShipProjectile& getProjectile();
private:
	void Init();

	BShipProjectile* projectile; //Lövedék pointer
	glm::mat4 projectileSharedTrans; //Lövedékkel azonos transzformácók

	int circleRes = 16; //körfelbontás

	gVertexBuffer	vb_cannon; //Ágyú grafikai modell adatai
	GLuint cannonTextureID; //Ágyú textúra azonosítója

	float alliedRotation = 0.0f; //forgatási érték ha a mi ágyunk
	glm::vec3 bShipTranslate; //csatahajó elmozgatása
	float cannonTranslateX; //mozgatás
	float cannonRotate = 0.25f * glm::pi<float>(); //forgatás
	glm::vec3 cannonRotateAngle = glm::vec3(0, 0, 1.0f); //forgatás tengelye
	glm::vec3 cannonScale = glm::vec3(0.125f, 0.5f, 0.125f) * TorpedoGLOBAL::Scale; //skálázás
};