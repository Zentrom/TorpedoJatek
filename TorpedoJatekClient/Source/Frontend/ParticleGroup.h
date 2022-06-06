#pragma once

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <vector>

//Részecske
class ParticleGroup
{
public:
	ParticleGroup(float genarea);
	~ParticleGroup();

	void Draw(gCamera &camera,gShaderProgram &sh_program, glm::vec3 projectilepos);
private:
	void GenerateGroup();
	void Init();
	glm::mat4 CalcToCameraRotation(gCamera &camera, glm::vec3 projectilepos);

	gVertexBuffer vb_particles; //Részecskecsoport modell adatai
	std::vector<glm::vec3> centerPoints; //Legenerált középpontok

	const int nrInGroup = 64; //Részecskeszám egy csoportban
	float generationArea; //Mekkora tartományba generálunk egy részecskecsoportot

	const float pRadius = 0.01f; //Egy részecske sugara
};