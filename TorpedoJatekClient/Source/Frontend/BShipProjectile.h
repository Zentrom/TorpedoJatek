#pragma once

#include "../Globals.hpp"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"
#include "../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//�gy� l�ved�ke
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

	int circleResHor = 8; //k�rfelbont�s v�zszintesen
	int circleResVert = 8; //k�rfelbont�s f�gg�legesen

	gVertexBuffer	vb_projectile; //L�ved�k grafikai modell adatai

	bool isAlly = false; //Mi l�ved�k�nk-e
	glm::mat4 cannonSharedTrans; //�gy�val k�z�s transzform�ci�k
	glm::vec3 projectileStartPos; //L�ved�k kiindul� pos
	glm::vec3 projectileCurrentPos; //L�ved�k jelenlegi pos
	glm::vec3 projectileTargetPos; //L�ved�k c�lpont
	float elapsedTime = 0.0f; //Anim�ci�b�l eltelt id�
	const float animationTime = 4.0f; //Anim�ci� ideje
	glm::vec3 dist; //Kezd� �s c�lpont k�zti t�v(y = x/4 -et kapja meg)

	glm::vec3 projectileScale = glm::vec3(0.125f) * TorpedoGLOBAL::Scale; //sk�l�z�s
};