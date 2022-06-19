#pragma once

#include "../Globals.hpp"
#include "ParticleGroup.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <vector>

//�gy� l�ved�ke
class BShipProjectile
{
public:
	BShipProjectile(void);
	BShipProjectile(const glm::mat4 &cannonsharedtrans, bool ally);
	~BShipProjectile(void);

	void Fire(glm::vec3 shottilepos);
	bool Animate(float deltatime);
	void Draw(gCamera& camera, glm::mat4 sharedtrans);

	ParticleGroup* getProjectileParticle();

private:
	void Init();
	glm::vec3 GetUV(float u, float v);

	int circleResHor = 8; //k�rfelbont�s v�zszintesen
	int circleResVert = 8; //k�rfelbont�s f�gg�legesen

	gVertexBuffer	vb_projectile; //L�ved�k grafikai modell adatai
	gShaderProgram sh_projectile; //R�szecsk�k shaderprogramja
	ParticleGroup* projectileParticle; //R�szecske
	std::vector<std::pair<glm::vec3,float>> particles; //L�ved�k ut�ni r�szecsk�k - <position,lifetime>
	float particleLife = 1.5f; //Meddig �l egy r�szecske

	bool isAlly = false; //Mi l�ved�k�nk-e
	glm::mat4 cannonSharedTrans; //�gy�val k�z�s transzform�ci�k
	glm::vec3 projectileStartPos; //L�ved�k kiindul� pos
	glm::vec3 projectileCurrentPos; //L�ved�k jelenlegi pos
	glm::vec3 projectileTargetPos; //L�ved�k c�lpont
	float elapsedTime = 0.0f; //Anim�ci�b�l eltelt id�
	const float animationTime = 4.0f; //Anim�ci� ideje
	float particleElapsed = 0.0f; //R�szecskeanim�ci�hoz
	const float particleEmitTime = 0.05f; //R�szecskesz�r�s ideje
	glm::vec3 dist; //Kezd� �s c�lpont k�zti t�v(y = x/4 -et kapja meg)

	glm::vec3 projectileScale = glm::vec3(0.125f) * TorpedoGLOBAL::Scale; //sk�l�z�s
};