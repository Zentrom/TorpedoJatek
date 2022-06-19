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

//Ágyú lövedéke
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

	int circleResHor = 8; //körfelbontás vízszintesen
	int circleResVert = 8; //körfelbontás függõlegesen

	gVertexBuffer	vb_projectile; //Lövedék grafikai modell adatai
	gShaderProgram sh_projectile; //Részecskék shaderprogramja
	ParticleGroup* projectileParticle; //Részecske
	std::vector<std::pair<glm::vec3,float>> particles; //Lövedék utáni részecskék - <position,lifetime>
	float particleLife = 1.5f; //Meddig él egy részecske

	bool isAlly = false; //Mi lövedékünk-e
	glm::mat4 cannonSharedTrans; //Ágyúval közös transzformációk
	glm::vec3 projectileStartPos; //Lövedék kiinduló pos
	glm::vec3 projectileCurrentPos; //Lövedék jelenlegi pos
	glm::vec3 projectileTargetPos; //Lövedék célpont
	float elapsedTime = 0.0f; //Animációból eltelt idõ
	const float animationTime = 4.0f; //Animáció ideje
	float particleElapsed = 0.0f; //Részecskeanimációhoz
	const float particleEmitTime = 0.05f; //Részecskeszórás ideje
	glm::vec3 dist; //Kezdõ és célpont közti táv(y = x/4 -et kapja meg)

	glm::vec3 projectileScale = glm::vec3(0.125f) * TorpedoGLOBAL::Scale; //skálázás
};