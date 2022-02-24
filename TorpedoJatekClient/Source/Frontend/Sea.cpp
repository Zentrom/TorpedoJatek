#include "Sea.h"

Sea::Sea(void)
{
	seaTiles.reserve(seaTileCount);
}

Sea::~Sea(void)
{
}

//Tengert inicializálja
void Sea::Init(int inMapSize)
{
	InitSeaTiles();
	InitPlayTiles(inMapSize);
	RemoveExtraSeaTiles();
	
	seaTileTextureID = GLUtils::TextureFromFile("Resources/Textures/seaTexture.bmp");
}

//Tenger animációhoz kellõ számítások
void Sea::Update(float deltatime)
{
	textureAnimationOffset += deltatime;
}

//Egy elõrajzolás,ami szükséges hogy tudjuk melyik játékmezõre mutatunk az egérrel
void Sea::PreProcess(gCamera &camera, gShaderProgram &sh_program)
{
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < (mapSize*mapSize); i++) {
		enemyTiles[i].PreProcess(camera, sh_program);
		myTiles[i].PreProcess(camera, sh_program);
	}
	glEnable(GL_CULL_FACE);
}

//Kirajzolja a játékmezõket
void Sea::Draw(gCamera &camera, gShaderProgram &sh_program)//, float pointedTile)
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glStencilMask(0xFF);
	//sh_program.SetUniform("read_index", static_cast<int>(pointedTile));
	sh_program.SetUniform("seatileOffset", textureAnimationOffset);
	for (int i = 0; i < (mapSize*mapSize); i++) {
		enemyTiles[i].Draw(camera, sh_program, seaTileTextureID);
		myTiles[i].Draw(camera, sh_program, seaTileTextureID);
	}
	glStencilMask(0x00);
	for (SeaTile &tile : seaTiles) {
		tile.Draw(camera, sh_program, seaTileTextureID);
	}
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

//Mindegyik játékmezõre meghívja a körvonal rajzolást
void Sea::OutlineDraw(gCamera &camera, gShaderProgram &sh_program, float pointedTile)
{
	glDisable(GL_CULL_FACE);
	sh_program.SetUniform("read_index", static_cast<int>(pointedTile));
	for (int i = 0; i < (mapSize*mapSize); i++) {
		enemyTiles[i].OutlineDraw(camera, sh_program);
		myTiles[i].OutlineDraw(camera, sh_program);
	}
	glEnable(GL_CULL_FACE);
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

//Játékmezõ indexe alapján adja vissza a játékmezõt
PlayTile& Sea::getTileByIndex(int index)
{
	for (PlayTile &tile : myTiles) {
		if (tile.getIndex() == index) {
			return tile;
		}
	}
	for (PlayTile &tile : enemyTiles) {
		if (tile.getIndex() == index) {
			return tile;
		}
	}
}

//Az ellenfél játékmezõindexei el vannak tolva,hogy mikor kiolvassuk akkor lehessen tudni
int Sea::getEnemyIndexOffset()
{
	return enemyTilesIndexOffset;
}

//Tenger összes mezõjét inicializálja
void Sea::InitSeaTiles()
{
	float seatile_transX = 0;
	float seatile_transZ = 0;
	glm::vec3 seatileResult = glm::vec3(0.0f);
	for (int i = 0; i < seaTileCount; i++) {
		seatile_transX = ((i % seaTileScale) * SeaTile::getScaleXZ() * TorpedoGLOBAL::Scale)
			- (SeaTile::getScaleXZ() * seaTileScale / 2.0f * TorpedoGLOBAL::Scale) + TorpedoGLOBAL::Scale;//Ide ezkell,hogy extra tileok kijöjjenek
		seatile_transZ = ((i / seaTileScale) * SeaTile::getScaleXZ() * TorpedoGLOBAL::Scale)
			- (SeaTile::getScaleXZ() * seaTileScale / 2.0f * TorpedoGLOBAL::Scale);//Itt nemkell,hogy az extra tileokat ki lehessen szedni

		seatileResult += glm::vec3(seatile_transX, 0, seatile_transZ);

		seaTiles.push_back(SeaTile(seatileResult));

		seatileResult = glm::vec3(0.0f);
	}
}

//Játékmezõket inicializálja
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
			enemyTiles[i*mapSize + j].setIndex(i*mapSize + j + enemyTilesIndexOffset);
		}
	}
}

//Kiszedi a azokat az extra tengermezõket,amik a játékmezõk helyén vannak
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

//Két játékmezõ elmozgatását nézi meg hogy float közelítõleg egyenlõ-e
bool Sea::CompareTileTranslations(SeaTile &sTile, PlayTile &pTile)
{
	return (round(sTile.getTranslate().x) == round(pTile.getTranslate().x)
		&& round(sTile.getTranslate().z) == round(pTile.getTranslate().z));
}

//Kiszámolja hogy hova kell elmozgatni egy játékmezõt grafikailag
glm::vec3 Sea::calcTranslate(int rowNr, int columnNr, bool ally) const
{
	float tile_transX = 0;
	float tile_transZ = 0;
	glm::vec3 tileResult = glm::vec3(0.0f);
	tile_transX = columnNr * SeaTile::getScaleXZ() * TorpedoGLOBAL::Scale;
	tile_transZ = (rowNr * SeaTile::getScaleXZ() * TorpedoGLOBAL::Scale) 
		- ((mapSize - 1) * SeaTile::getScaleXZ() / 2.0f * TorpedoGLOBAL::Scale);

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


