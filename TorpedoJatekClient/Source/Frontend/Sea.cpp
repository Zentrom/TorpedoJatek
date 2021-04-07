#include "Sea.h"

#include <iostream>

Sea::Sea(void)
{
	//myPlayTiles.reserve(25);
	//enemyPlayTiles.reserve(25);
}

Sea::~Sea(void)
{
}


//void Sea::Init()
//{
//	float tile_transX = 0;
//	float tile_transZ = 0;
//	glm::vec3 tileResult = glm::vec3(0.0f);
//	for (int i = 0; i < mapSize; i++) {
//		tile_transX = (i % TorpedoGLOBAL::MapSize) * 2.0f * TorpedoGLOBAL::Scale;
//		tile_transZ = ((i / TorpedoGLOBAL::MapSize) * 2.0f * TorpedoGLOBAL::Scale) - (3.0f*2.0f * TorpedoGLOBAL::Scale);
//
//		tileResult += glm::vec3(mountaincenter_border_Xoffset, 0, 0) +
//			glm::vec3(mountain_tile_offset, 0, 0) +
//			glm::vec3(tile_transX, 0, tile_transZ);
//
//		enemyPlayTiles[i] = PlayTile(tileResult);
//		myPlayTiles[i] = PlayTile(glm::vec3(-1, 1, 1)*tileResult);
//
//		enemyPlayTiles[i].Init();
//		myPlayTiles[i].Init();
//		tileResult = glm::vec3(0.0f);
//	}
//}

void Sea::Init(int inMapSize)
{
	mapSize = inMapSize;
	tileCount = mapSize*mapSize;
	myTiles.reserve(tileCount); 
	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++) {
			myTiles.push_back(PlayTile(std::pair<char,int>(static_cast<char>('a'+i),1+j)));
			myTiles[i*mapSize+j].setTranslate(calcTranslate(i,j,true));
			myTiles[i*mapSize+j].setIndex(i*mapSize+j);
		}
	}

	enemyTiles.reserve(tileCount);
	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++) {
			enemyTiles.push_back(PlayTile(std::pair<char, int>(static_cast<char>('a' + i),1+j)));
			enemyTiles[i*mapSize+j].setTranslate(calcTranslate(i, j, false));
			enemyTiles[i*mapSize+j].setIndex(i*mapSize+j);
		}
	}
	firstTile_battleShipOffset = 2.0f*mapSize*TorpedoGLOBAL::Scale;
}

void Sea::Draw(gCamera &camera, gShaderProgram &sh_program)
{

	for (int i = 0; i < (mapSize*mapSize); i++) {
		enemyTiles[i].Draw(camera, sh_program);
		myTiles[i].Draw(camera, sh_program);
	}

}

std::vector<PlayTile>& Sea::getTiles(bool ally)
{
	if (ally) {
		return myTiles;
	}
	else {
		return enemyTiles;
	}
}

glm::vec3 Sea::calcTranslate(int rowNr, int columnNr,bool ally)
{
	float tile_transX = 0;
	float tile_transZ = 0;
	glm::vec3 tileResult = glm::vec3(0.0f);
	tile_transX = columnNr * 2.0f * TorpedoGLOBAL::Scale;
	tile_transZ = (rowNr * 2.0f * TorpedoGLOBAL::Scale) - ((mapSize-1)/2.0f*2.0f * TorpedoGLOBAL::Scale);

	//std::cout << tile_transX << " - " << tile_transZ << std::endl;

	tileResult += glm::vec3(mountaincenter_border_Xoffset, 0, 0) +
		glm::vec3(mountain_tile_offset, 0, 0) +
		glm::vec3(tile_transX, 0, tile_transZ);

	//enemyPlayTiles[i] = PlayTile(tileResult);
	//myPlayTiles[i] = PlayTile(glm::vec3(-1, 1, 1)*tileResult);

	if (ally) {
		tileResult = glm::vec3(-1, 1, 1)*tileResult;
	}
	else {
		tileResult = glm::vec3(1, 1, -1)*tileResult;
	}
	return tileResult;
}


