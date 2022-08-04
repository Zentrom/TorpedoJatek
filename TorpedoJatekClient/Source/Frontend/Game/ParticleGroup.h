#pragma once

#include "../../Globals.hpp"

#include "../../../Utility/gVertexBuffer.h"
#include "../../../Utility/gShaderProgram.h"
#include "../../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <vector>

//Részecskék csoportja
class ParticleGroup
{
public:
	ParticleGroup(float generation_area);
	~ParticleGroup();

	void Draw(const gCamera& camera, gShaderProgram& sh_program, const glm::vec3& projectile_pos) const;
private:
	void Init();
	void GenerateGroup();
	const glm::mat4 CalcToCameraRotation(const gCamera& camera, const glm::vec3& projectile_pos) const;

	gVertexBuffer vb_particles; //Részecskecsoport modell adatai
	std::vector<glm::vec3> centerPoints; //Legenerált középpontok

	const glm::vec3 objectLookAt = glm::vec3(0, 0, -1.0f); //Alapból merre néznek a részecskék
	const int nrInGroup = 64; //Részecskeszám egy csoportban
	const float radius = 0.01f * TorpedoGLOBAL::Scale; //Egy részecske sugara
	const float color = 0.8f; //Szín

	const float generationArea; //Mekkora tartományba generálunk egy részecskecsoportot
};