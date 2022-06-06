#include "ParticleGroup.h"

ParticleGroup::ParticleGroup(float genarea) : generationArea(genarea)
{
	GenerateGroup();
	Init();
}

ParticleGroup::~ParticleGroup()
{
}

//Legenerál egy részecskecsoport középpontjait
void ParticleGroup::GenerateGroup()
{
	glm::vec3 generatedPoint;
	for (int i = 0; i < nrInGroup; i++) {
		generatedPoint.x = (generationArea * 2 * (std::rand() / (float)RAND_MAX)) - generationArea;
		generatedPoint.y = (generationArea * 2 * (std::rand() / (float)RAND_MAX)) - generationArea;
		generatedPoint.z = (generationArea * 2 * (std::rand() / (float)RAND_MAX)) - generationArea;

		centerPoints.push_back(generatedPoint);
	}
}

void ParticleGroup::Init()
{
	vb_particles.AddAttribute(0, 3); //pos
	vb_particles.AddAttribute(1, 3); //color

	for (int i = 0; i < nrInGroup; i++) {
		vb_particles.AddData(0, centerPoints.at(i).x - pRadius, centerPoints.at(i).y + pRadius, centerPoints.at(i).z);
		vb_particles.AddData(0, centerPoints.at(i).x + pRadius, centerPoints.at(i).y + pRadius, centerPoints.at(i).z);
		vb_particles.AddData(0, centerPoints.at(i).x - pRadius, centerPoints.at(i).y - pRadius, centerPoints.at(i).z);
		vb_particles.AddData(0, centerPoints.at(i).x + pRadius, centerPoints.at(i).y - pRadius, centerPoints.at(i).z);

		for (int i = 0; i < 4; i++) {
			vb_particles.AddData(1, 0.8f, 0.8f, 0.8f);
		}
	}

	vb_particles.InitBuffers();
}

//Kamera felé nézõ forgatás - Cylindrical Billboarding
glm::mat4 ParticleGroup::CalcToCameraRotation(gCamera &camera, glm::vec3 projectilepos)
{
	//XZ-ben
	glm::vec3 objectToCamera;
	objectToCamera.x = camera.GetEye().x - projectilepos.x;
	objectToCamera.y = 0;
	objectToCamera.z = camera.GetEye().z - projectilepos.z;
	glm::vec3 objectLookAt = glm::vec3(0, 0, -1.0f);
	glm::vec3 normalizedOTC = glm::normalize(objectToCamera);

	glm::vec3 objectUp = glm::cross(objectLookAt, normalizedOTC);
	float angle = glm::dot(objectLookAt, normalizedOTC);

	glm::mat4 resultRotation = glm::rotate(acosf(angle), glm::vec3(objectUp.x, objectUp.y, objectUp.z));

	return resultRotation;
}

//Részecskecsoport kirajzolása
void ParticleGroup::Draw(gCamera &camera, gShaderProgram &sh_program, glm::vec3 projectilepos)
{
	glm::mat4 matWorld = glm::translate(projectilepos) * CalcToCameraRotation(camera, projectilepos);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);

	vb_particles.On();
	for (int i = 0; i < nrInGroup; i++) {
		vb_particles.Draw(GL_TRIANGLE_STRIP, i * 4, 4);
	}
	vb_particles.Off();
}
