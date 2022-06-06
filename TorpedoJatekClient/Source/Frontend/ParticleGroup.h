#pragma once

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <vector>

//R�szecske
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

	gVertexBuffer vb_particles; //R�szecskecsoport modell adatai
	std::vector<glm::vec3> centerPoints; //Legener�lt k�z�ppontok

	const int nrInGroup = 64; //R�szecskesz�m egy csoportban
	float generationArea; //Mekkora tartom�nyba gener�lunk egy r�szecskecsoportot

	const float pRadius = 0.01f; //Egy r�szecske sugara
};