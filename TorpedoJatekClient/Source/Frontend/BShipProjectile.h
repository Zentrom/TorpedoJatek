#pragma once

#include "../Globals.hpp"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"
#include "../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Ágyú lövedéke
class BShipProjectile
{
public:
	BShipProjectile(void);
	BShipProjectile(const glm::mat4 &cannonsharedtrans, bool ally);
	~BShipProjectile(void);

	void Fire(glm::vec3 shottilepos);
	bool Animate(float deltatime);
	void Draw(gCamera &camera, gShaderProgram &sh_program);

private:
	void Init();
	glm::vec3 GetUV(float u, float v);

	int circleResHor = 8; //körfelbontás vízszintesen
	int circleResVert = 8; //körfelbontás függõlegesen

	gVertexBuffer	vb_projectile; //Lövedék grafikai modell adatai

	bool isAlly = false; //Mi lövedékünk-e
	glm::mat4 cannonSharedTrans; //Ágyúval közös transzformációk
	glm::vec3 projectileStartPos; //Lövedék kiinduló pos
	glm::vec3 projectileCurrentPos; //Lövedék jelenlegi pos
	glm::vec3 projectileTargetPos; //Lövedék célpont
	float elapsedTime = 0.0f; //Animációból eltelt idõ
	const float animationTime = 4.0f; //Animáció ideje
	glm::vec3 dist; //Kezdõ és célpont közti táv(y = x/4 -et kapja meg)

	glm::vec3 projectileScale = glm::vec3(0.125f) * TorpedoGLOBAL::Scale; //skálázás
};