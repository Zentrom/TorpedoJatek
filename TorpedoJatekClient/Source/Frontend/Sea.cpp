#include "Sea.h"

Sea::Sea(void)
{
}

Sea::~Sea(void)
{
}

//Játékpálya mérete alapján inicializája a játékmezõket
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
	mountaincenter_border_Xoffset = (mapSize - 1) / 2.0f*TorpedoGLOBAL::Scale;
}

//Kirajzolja a játékmezõket
void Sea::Draw(gCamera &camera, gShaderProgram &sh_program)
{
	for (int i = 0; i < (mapSize*mapSize); i++) {
		enemyTiles[i].Draw(camera, sh_program);
		myTiles[i].Draw(camera, sh_program);
	}
}

//Visszaadja egy játékos játékmezõit
std::vector<PlayTile>& Sea::getTiles(bool ally)
{
	if (ally) {
		return myTiles;
	}
	else {
		return enemyTiles;
	}
}

//Kiszámolja hogy hova kell elmozgatni egy játékmezõt grafikailag
glm::vec3 Sea::calcTranslate(int rowNr, int columnNr,bool ally)
{
	float tile_transX = 0;
	float tile_transZ = 0;
	glm::vec3 tileResult = glm::vec3(0.0f);
	tile_transX = columnNr * 2.0f * TorpedoGLOBAL::Scale;
	tile_transZ = (rowNr * 2.0f * TorpedoGLOBAL::Scale) - ((mapSize-1)*TorpedoGLOBAL::Scale);

	tileResult += glm::vec3(mountaincenter_border_Xoffset, 0, 0) +
		glm::vec3(mountain_tile_offset, 0, 0) +
		glm::vec3(tile_transX, 0, tile_transZ);

	if (ally) {
		tileResult = glm::vec3(-1, 1, 1)*tileResult;
	}
	else {
		tileResult = glm::vec3(1, 1, -1)*tileResult;
	}
	return tileResult;
}


