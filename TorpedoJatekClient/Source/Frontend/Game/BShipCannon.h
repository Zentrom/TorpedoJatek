#pragma once

#include "../../Globals.hpp"
#include "BShipProjectile.h"

#include "../../../Utility/gVertexBuffer.h"
#include "../../../Utility/gShaderProgram.h"
#include "../../../Utility/gCamera.h"
#include "../../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Csatahajó ágyúja
class BShipCannon
{
public:
	BShipCannon(const glm::vec3& battle_ship_translate,bool ally, float battle_ship_scale_x);
	~BShipCannon();

	void Draw(const gCamera& camera, gShaderProgram& sh_program, const glm::mat4& shared_trans) const;
	BShipProjectile& getProjectile() const;
private:
	void Init();

	BShipProjectile* projectile; //Lövedék pointer
	gVertexBuffer	vb_cannon; //Ágyú grafikai modell adatai
	GLuint cannonTextureID; //Ágyú textúra azonosítója

	const int circleRes = 16; //körfelbontás
	const float cannonBShipDivFactor = 8.0f; //Ágyú mozgatása hajó területén belül(osztás)
	const float cannonTranslateY = 0.25f; //Mozgatás Y tengelyen

	const float cannonRotate = 0.25f * glm::pi<float>(); //forgatás
	const glm::vec3 cannonRotateAngle = glm::vec3(0, 0, 1.0f); //forgatás tengelye
	const glm::vec3 cannonScale = glm::vec3(0.125f, 0.5f, 0.125f) * TorpedoGLOBAL::Scale; //skálázás

	glm::mat4 projectileSharedTrans; //Lövedékkel azonos transzformációk

	const glm::vec3 bShipTranslate; //csatahajó elmozgatása
	const float alliedRotation; //forgatási érték ha a mi ágyunk
	const float cannonTranslateX; //mozgatás X tengelyen
};