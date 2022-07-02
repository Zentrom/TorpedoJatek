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

//�gy� l�ved�ke
class BShipProjectile
{
public:
	BShipProjectile(const glm::mat4& cannon_shared_trans, bool ally);
	~BShipProjectile();

	void Fire(const glm::vec3& shot_tile_pos);
	bool Animate(float delta_time);
	void Draw(const gCamera& camera,const glm::mat4& shared_trans);

private:
	void Init();
	glm::vec3 GetUV(float u, float v);

	gVertexBuffer vb_projectile; //L�ved�k grafikai modell adatai
	gShaderProgram sh_projectile; //R�szecsk�k shaderprogramja
	std::vector<std::pair<const glm::vec3,float>> particles; //L�ved�k ut�ni r�szecsk�k - <position,lifetime>

	const int circleResHor = 16; //k�rfelbont�s v�zszintesen
	const int circleResVert = 16; //k�rfelbont�s f�gg�legesen
	const float animationTime = 4.0f; //Anim�ci� ideje
	float elapsedTime = 0.0f; //Anim�ci�b�l eltelt id�
	const float particleLife = 1.5f; //Meddig �l egy r�szecske
	float particleElapsed = 0.0f; //R�szecskeanim�ci�hoz
	const float particleEmitTime = 0.05f; //R�szecskesz�r�s ideje
	const float distHeightDivFactor = 4.0f; //T�vols�g f�ggv�ny�ben mennyire magasra megy�nk(oszt�si faktor)

	const glm::vec3 projectileScale = glm::vec3(0.125f) * TorpedoGLOBAL::Scale; //sk�l�z�s

	glm::vec3 dist; //Kezd� �s c�lpont k�zti t�v(y = x/4 -et kapja meg)
	glm::vec3 currentPos; //L�ved�k jelenlegi pos
	glm::vec3 targetPos; //L�ved�k c�lpont

	const glm::vec3 startPos; //L�ved�k kiindul� pos
	const bool isAlly; //Mi l�ved�k�nk-e
	ParticleGroup* const projectileParticle; //R�szecskecsoport
};