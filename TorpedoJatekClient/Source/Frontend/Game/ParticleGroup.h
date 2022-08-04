#pragma once

#include "../../Globals.hpp"

#include "../../../Utility/gVertexBuffer.h"
#include "../../../Utility/gShaderProgram.h"
#include "../../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <vector>

//R�szecsk�k csoportja
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

	gVertexBuffer vb_particles; //R�szecskecsoport modell adatai
	std::vector<glm::vec3> centerPoints; //Legener�lt k�z�ppontok

	const glm::vec3 objectLookAt = glm::vec3(0, 0, -1.0f); //Alapb�l merre n�znek a r�szecsk�k
	const int nrInGroup = 64; //R�szecskesz�m egy csoportban
	const float radius = 0.01f * TorpedoGLOBAL::Scale; //Egy r�szecske sugara
	const float color = 0.8f; //Sz�n

	const float generationArea; //Mekkora tartom�nyba gener�lunk egy r�szecskecsoportot
};