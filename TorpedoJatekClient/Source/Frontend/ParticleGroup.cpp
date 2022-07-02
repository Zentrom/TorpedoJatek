#include "ParticleGroup.h"

ParticleGroup::ParticleGroup(float generation_area) : generationArea(generation_area)
{
	GenerateGroup();
	Init();
}

ParticleGroup::~ParticleGroup()
{
	vb_particles.Clean();
}

//Legenerál egy részecskecsoport középpontjait
void ParticleGroup::GenerateGroup()
{
	glm::vec3 generatedPoint;
	for (int i = 0; i < nrInGroup; ++i) {
		generatedPoint.x = (generationArea * 2 * (std::rand() / (float)RAND_MAX)) - generationArea;
		generatedPoint.y = (generationArea * 2 * (std::rand() / (float)RAND_MAX)) - generationArea;
		generatedPoint.z = (generationArea * 2 * (std::rand() / (float)RAND_MAX)) - generationArea;

		centerPoints.push_back(generatedPoint);
	}
}

//Inicializálja a részecskecsoportot
void ParticleGroup::Init()
{
	vb_particles.AddAttribute(0, 3); //pos
	vb_particles.AddAttribute(1, 3); //color

	for (int i = 0; i < nrInGroup; ++i) {
		vb_particles.AddData(0, centerPoints.at(i).x - radius, centerPoints.at(i).y + radius, centerPoints.at(i).z);
		vb_particles.AddData(0, centerPoints.at(i).x + radius, centerPoints.at(i).y + radius, centerPoints.at(i).z);
		vb_particles.AddData(0, centerPoints.at(i).x - radius, centerPoints.at(i).y - radius, centerPoints.at(i).z);
		vb_particles.AddData(0, centerPoints.at(i).x + radius, centerPoints.at(i).y - radius, centerPoints.at(i).z);

		for (int i = 0; i < 4; ++i) {
			vb_particles.AddData(1, color, color, color);
		}
	}

	vb_particles.InitBuffers();
}

//Kamera felé nézõ forgatás - Cylindrical Billboarding
const glm::mat4 ParticleGroup::CalcToCameraRotation(const gCamera& camera, const glm::vec3& projectile_pos) const
{
	//XZ-ben
	glm::vec3 objectToCamera = glm::vec3(camera.GetEye().x - projectile_pos.x, 0,
		camera.GetEye().z - projectile_pos.z);
	glm::vec3 normalizedOTC = glm::normalize(objectToCamera);

	glm::vec3 objectUp = glm::cross(objectLookAt, normalizedOTC);
	float angle = glm::dot(objectLookAt, normalizedOTC);

	glm::mat4 resultRotation = glm::rotate(acosf(angle), objectUp);

	return resultRotation;
}

//Részecskecsoport kirajzolása
void ParticleGroup::Draw(const gCamera& camera, gShaderProgram& sh_program, const glm::vec3& projectilepos) const
{
	glm::mat4 matWorld = glm::translate(projectilepos) * CalcToCameraRotation(camera, projectilepos);
	glm::mat4 mvp = camera.GetViewProj() * matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("MVP", mvp);

	vb_particles.On();
	for (int i = 0; i < nrInGroup; ++i) {
		vb_particles.Draw(GL_TRIANGLE_STRIP, i * 4, 4);
	}
	vb_particles.Off();
}
