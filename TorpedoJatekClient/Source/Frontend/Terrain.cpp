
#include "Terrain.h"

//matWorld itt a VB-be beégetett traszok után hat, hagyjuk így ha lehet
Terrain::Terrain(int seatile_row_count, float mountain_height_y) : 
	groundScaleToSeaTile(seatile_row_count / static_cast<float>(terrainScale)),
	groundMountainY((mountain_height_y / -2.0f - 0.01f) * TorpedoGLOBAL::Scale),
	terrainSizeXZ(terrainScale*terrainScale), matWorld(glm::mat4(1.0f)),
	matWorldIT(glm::transpose(glm::inverse(matWorld))),
	groundScaleXZ(TorpedoGLOBAL::SeaTileScaleXZ * groundScaleToSeaTile * TorpedoGLOBAL::Scale)
{
	groundTileTrans.reserve(terrainSizeXZ);
}

Terrain::~Terrain()
{
	vb_terrain.Clean();
	glDeleteTextures(1, &groundTextureID);
}

//Inicializálja a Földet összerakó darabokat
void Terrain::Init()
{
	if (TorpedoGLOBAL::Debug) {
		std::cout << "-----------------------------------" << std::endl;
		std::cout << "Terrain::groundScaleToSeatile: " << groundScaleToSeaTile << std::endl;
	}

	CalcGroundTileTransformations();

	vb_terrain.AddAttribute(0, 3); //pos
	vb_terrain.AddAttribute(1, 4); //szín
	vb_terrain.AddAttribute(2, 3); //normál
	vb_terrain.AddAttribute(3, 2); //textcoord

	for (int i = 0; i < terrainSizeXZ; ++i) {
		vb_terrain.AddData(0, groundTileTrans.at(i).x - 0.5f * groundScaleXZ, groundTileTrans.at(i).y,
			groundTileTrans.at(i).z - 0.5f * groundScaleXZ);
		vb_terrain.AddData(0, groundTileTrans.at(i).x + 0.5f * groundScaleXZ, groundTileTrans.at(i).y,
			groundTileTrans.at(i).z - 0.5f * groundScaleXZ);
		vb_terrain.AddData(0, groundTileTrans.at(i).x - 0.5f * groundScaleXZ, groundTileTrans.at(i).y,
			groundTileTrans.at(i).z + 0.5f * groundScaleXZ);
		vb_terrain.AddData(0, groundTileTrans.at(i).x + 0.5f * groundScaleXZ, groundTileTrans.at(i).y,
			groundTileTrans.at(i).z + 0.5f * groundScaleXZ);

		for (int j = 0; j < 4; ++j) {
			vb_terrain.AddData(1, 0.4f, 0.4f, 0, 1);
			vb_terrain.AddData(2, 0, 1, 0);
		}

		vb_terrain.AddData(3, 0, 0);
		vb_terrain.AddData(3, 1, 0);
		vb_terrain.AddData(3, 0, 1);
		vb_terrain.AddData(3, 1, 1);

		vb_terrain.AddIndex((i * 4) + 1, (i * 4) + 0, (i * 4) + 2);
		vb_terrain.AddIndex((i * 4) + 1, (i * 4) + 2, (i * 4) + 3);
	}

	vb_terrain.InitBuffers();
	groundTileTrans.clear();

	groundTextureID = GLUtils::TextureFromFile("Resources/Textures/groundTexture.bmp");
}

//Kiszámolja hogy mennyivel kell elmozgatni minden egyes Föld darabot
void Terrain::CalcGroundTileTransformations()
{
	float ground_transX = 0;
	float ground_transZ = 0;
	glm::vec3 groundResult = glm::vec3(0.0f);
	for (int i = 0; i < terrainSizeXZ; ++i) {
		ground_transX = ((i % terrainScale) * groundScaleXZ)
			//+ AZÉRT VAN HOGY CENTEREZVE LEGYEN CSAK MIÉRT ENNYIVEL PONT?? Ahányszor nagyobb a seatile
			- (groundScaleXZ * terrainScale / 2.0f) + (groundScaleXZ / 4.0f);
		ground_transZ = ((i / terrainScale) * groundScaleXZ)
			- (groundScaleXZ * terrainScale / 2.0f) + (groundScaleXZ / 4.0f);

		//if (i == 0) {
		//	std::cout << "Ground: " << ground_transX << " " << ground_transZ << std::endl;
		//}

		groundResult += glm::vec3(ground_transX, groundMountainY, ground_transZ);
		//std::cout << ground_transX << " ts "  << terrainScale << " gr " << groundScaleXZ << " ";
		groundTileTrans.push_back(groundResult);
		groundResult = glm::vec3(0.0f);
	}
}

//A Föld terület kirajzolása
void Terrain::Draw(const gCamera& camera, gShaderProgram& sh_program) const
{
	glm::mat4 mvp = camera.GetViewProj() * matWorld;

	sh_program.SetUniform("hasTexture", true);
	sh_program.SetTexture("texImage", 0, groundTextureID);
	
	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	sh_program.SetUniform("eye_pos", camera.GetEye());

	vb_terrain.On();
	vb_terrain.DrawIndexed(GL_TRIANGLES, 0, 6 * terrainSizeXZ);
	vb_terrain.Off();
	sh_program.SetUniform("hasTexture", false);
}

const int Terrain::getTerrainScale() const
{
	return terrainScale;
}

const float Terrain::getGroundScaleXZ() const
{
	return groundScaleXZ;
}
