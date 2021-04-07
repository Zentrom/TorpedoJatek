
#include "Terrain.h"

Terrain::Terrain(void)
{
	myGrounds = new Ground[terrainSize];

	float ground_transX = 0;
	float ground_transZ = 0;
	float ground_mountainY = -3.01f * TorpedoGLOBAL::Scale;
	glm::vec3 groundResult = glm::vec3(0.0f);
	for (int i = 0; i < terrainSize; i++) {
		ground_transX = ((i % (mapSize* terrainScale)) * 2.0f*TorpedoGLOBAL::Scale) - (3.0f*2.0f*TorpedoGLOBAL::Scale*terrainScale)-((terrainScale-1)*TorpedoGLOBAL::Scale);
		ground_transZ = ((i / (mapSize* terrainScale)) * 2.0f*TorpedoGLOBAL::Scale) - (3.0f*2.0f*TorpedoGLOBAL::Scale*terrainScale)-((terrainScale-1)*TorpedoGLOBAL::Scale);

		groundResult += glm::vec3(ground_transX, ground_mountainY, ground_transZ);

		myGrounds[i] = Ground(groundResult);
		
		groundResult = glm::vec3(0.0f);
	}
}

Terrain::~Terrain(void)
{
	delete[] myGrounds;
}

//Inicializálja a Földet összerakó darabokat
void Terrain::Init()
{
	for (int i = 0; i < terrainSize; i++) {
		myGrounds[i].Init();
	}
}

//A földdarabok kirajzolása
void Terrain::Draw(gCamera &camera, gShaderProgram &sh_program)
{
	for (int i = 0; i < terrainSize; i++) {
		myGrounds[i].Draw(camera,sh_program);
	}
}