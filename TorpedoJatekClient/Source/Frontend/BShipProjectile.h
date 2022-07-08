#pragma once

#include "../Globals.hpp"
#include "ParticleGroup.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <iostream>
#include <vector>

//Ágyú lövedéke
class BShipProjectile
{
public:
	BShipProjectile(const glm::mat4& cannon_shared_trans, bool ally);
	~BShipProjectile();

	void Fire(const glm::vec3& shot_tile_pos);
	bool Animate(float delta_time);
	bool AnimPostParticles(float delta_time);
	void ClearParticles();
	void Draw(const gCamera& camera,const glm::mat4& shared_trans);
	float getAnimationTime() const;

private:
	void Init();
	glm::vec3 GetUV(float u, float v);

	gVertexBuffer vb_projectile; //Lövedék grafikai modell adatai
	gShaderProgram sh_projectile; //Részecskék shaderprogramja
	std::vector<std::pair<const glm::vec3,float>> particles; //Lövedék utáni részecskék - <position,lifetime>

	const int circleResHor = 16; //körfelbontás vízszintesen
	const int circleResVert = 16; //körfelbontás függõlegesen
	const float animationTime = 0.5f;//4.0f; //Animáció ideje
	const float particleLife = 1.5f; //Meddig él egy részecske
	const float particleEmitTime = 0.05f; //Részecskeszórás ideje
	const float distHeightDivFactor = 4.0f; //Távolság függvényében mennyire magasra megyünk(osztási faktor)

	const glm::vec3 projectileScale = glm::vec3(0.125f) * TorpedoGLOBAL::Scale; //skálázás
	glm::vec3 dist; //Kezdõ és célpont közti táv(y = x/4 -et kapja meg)
	glm::vec3 currentPos; //Lövedék jelenlegi pos
	glm::vec3 targetPos; //Lövedék célpont

	float elapsedTime = 0.0f; //Animációból eltelt idõ
	float particleElapsed = 0.0f; //Részecskeanimáció szórásához
	float postAnimParticleElapsed = 0.0f; //Animáció bejezetével füst animációhoz számláló

	const glm::vec3 startPos; //Lövedék kiinduló pos
	const bool isAlly; //Mi lövedékünk-e
	ParticleGroup* const projectileParticle; //Részecskecsoport
	const float postAnimParticleTime; //Animáció befejeztével meddig folytatjuk a füstanimációt
};