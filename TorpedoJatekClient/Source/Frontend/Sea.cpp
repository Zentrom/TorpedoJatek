#include "Sea.h"

Sea::Sea(void)
{
	seaTiles.reserve(seaTileCount);
}

Sea::~Sea(void)
{
}

//Tengert inicializ�lja
void Sea::Init(int inMapSize)
{
	InitSeaTiles();
	InitPlayTiles(inMapSize);
	RemoveExtraSeaTiles();
}

//Kirajzolja a j�t�kmez�ket
void Sea::Draw(gCamera &camera, gShaderProgram &sh_program)
{
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < (mapSize*mapSize); i++) {
		enemyTiles[i].Draw(camera, sh_program);
		myTiles[i].Draw(camera, sh_program);
	}
	for (SeaTile &tile : seaTiles) {
		tile.Draw(camera, sh_program);
	}
	glEnable(GL_CULL_FACE);
}

//Visszaadja egy j�t�kos j�t�kmez�it
std::vector<PlayTile>& Sea::getTiles(bool ally)
{
	if (ally) {
		return myTiles;
	}
	else {
		return enemyTiles;
	}
}

//Tenger �sszes mez�j�t inicializ�lja
void Sea::InitSeaTiles()
{
	float seatile_transX = 0;
	float seatile_transZ = 0;
	glm::vec3 seatileResult = glm::vec3(0.0f);
	for (int i = 0; i < seaTileCount; i++) {
		seatile_transX = ((i % seaTileScale) * SeaTile::getScaleXZ() * TorpedoGLOBAL::Scale)
			- (SeaTile::getScaleXZ() * seaTileScale / 2.0f * TorpedoGLOBAL::Scale) + (SeaTile::getScaleXZ() / 2.0f);
		seatile_transZ = ((i / seaTileScale) * SeaTile::getScaleXZ() * TorpedoGLOBAL::Scale)
			- (SeaTile::getScaleXZ() * seaTileScale / 2.0f * TorpedoGLOBAL::Scale);//Itt nemkell,hogy az extra tileokat ki lehessen szedni + (SeaTile::getScaleXZ() / 2.0f);

		seatileResult += glm::vec3(seatile_transX, 0, seatile_transZ);

		seaTiles.push_back(SeaTile(seatileResult));

		seatileResult = glm::vec3(0.0f);
	}
}

//J�t�kmez�ket inicializ�lja
void Sea::InitPlayTiles(int inMapSize)
{
	mapSize = inMapSize;
	playTileCount = mapSize*mapSize;
	myTiles.reserve(playTileCount);
	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++) {
			myTiles.push_back(PlayTile(std::pair<char, int>(static_cast<char>('a' + i), 1 + j)));
			myTiles[i*mapSize + j].setTranslate(calcTranslate(i, j, true));
			myTiles[i*mapSize + j].setIndex(i*mapSize + j);
		}
	}

	enemyTiles.reserve(playTileCount);
	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++) {
			enemyTiles.push_back(PlayTile(std::pair<char, int>(static_cast<char>('a' + i), 1 + j)));
			enemyTiles[i*mapSize + j].setTranslate(calcTranslate(i, j, false));
			enemyTiles[i*mapSize + j].setIndex(i*mapSize + j);
		}
	}
}

//Kiszedi a azokat az extra tengermez�ket,amik a j�t�kmez�k hely�n vannak
void Sea::RemoveExtraSeaTiles()
{
	int initialSize = seaTiles.size();
	bool foundInMyTiles;
	for (std::vector<SeaTile>::iterator it = seaTiles.begin(); it != seaTiles.end();++it) 
	{
		foundInMyTiles = false;
		for (PlayTile pTile : myTiles) {
			if (CompareTileTranslations(*it,pTile)) {
				seaTiles.erase(it);
				--it;
				break;
			}
		}
		if (foundInMyTiles) {
			foundInMyTiles = false;
			continue;
		}
		for (PlayTile pTile : enemyTiles) {
			if (CompareTileTranslations(*it, pTile)) {
				seaTiles.erase(it);
				--it;
				break;
			}
		}
	}
	if (initialSize - 2 * myTiles.size() != seaTiles.size()) {
		std::cout << "Warning: Extra SeaTiles were not removed correctly! initial: " << initialSize 
			<< " after: " << seaTiles.size() << std::endl;
	}
}

bool Sea::CompareTileTranslations(SeaTile &sTile, PlayTile &pTile)
{
	return (round(sTile.getTranslate().x) == round(pTile.getTranslate().x)
		&& round(sTile.getTranslate().z) == round(pTile.getTranslate().z));
}

//Kisz�molja hogy hova kell elmozgatni egy j�t�kmez�t grafikailag
glm::vec3 Sea::calcTranslate(int rowNr, int columnNr, bool ally) const
{
	float tile_transX = 0;
	float tile_transZ = 0;
	glm::vec3 tileResult = glm::vec3(0.0f);
	tile_transX = columnNr * SeaTile::getScaleXZ() * TorpedoGLOBAL::Scale;
	tile_transZ = (rowNr * SeaTile::getScaleXZ() * TorpedoGLOBAL::Scale) - ((mapSize - 1)*TorpedoGLOBAL::Scale);

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


