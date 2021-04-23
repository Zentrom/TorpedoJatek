
#include "Terrain.h"
#include "../../Utility/GLUtils.hpp"

Terrain::Terrain(void)
{
	myGrounds.reserve(terrainSizeXZ);

	float ground_transX = 0;
	float ground_transZ = 0;
	glm::vec3 groundResult = glm::vec3(0.0f);
	for (int i = 0; i < terrainSizeXZ; i++) {
		ground_transX = ((i % terrainScale) * Ground::getScaleXZ() * TorpedoGLOBAL::Scale)
			- (Ground::getScaleXZ() * terrainScale / 2.0f * TorpedoGLOBAL::Scale) + (Ground::getScaleXZ() / 2.0f);
		ground_transZ = ((i / terrainScale) * Ground::getScaleXZ() * TorpedoGLOBAL::Scale)
			- (Ground::getScaleXZ() * terrainScale / 2.0f * TorpedoGLOBAL::Scale) + (Ground::getScaleXZ() / 2.0f);

		groundResult += glm::vec3(ground_transX, ground_mountainY, ground_transZ);

		myGrounds.push_back(Ground(groundResult));

		groundResult = glm::vec3(0.0f);
	}
}

Terrain::~Terrain(void)
{
	glDeleteTextures(1, &groundTextureID);
}

//Inicializálja a Földet összerakó darabokat
void Terrain::Init()
{
	for (Ground &ground : myGrounds) {
		ground.Init();
	}
	groundTextureID = TextureFromFile("Resources/Textures/groundTexture.bmp");
}

//A földdarabok kirajzolása
void Terrain::Draw(gCamera &camera, gShaderProgram &sh_program)
{
	for (Ground &ground : myGrounds) {
		ground.Draw(camera, sh_program, groundTextureID);
	}
}

int Terrain::getTerrainScale()
{
	return terrainScale;
}
