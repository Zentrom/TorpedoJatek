#include "Sea.h"

//const float Sea::tileScaleXZ = 2.0f; //ez most 1 és 2-re megy

Sea::Sea() : matWorld(glm::mat4(1.0f)),
	matWorldIT(glm::transpose(glm::inverse(matWorld)))
{
	//seaTiles.reserve(seaTileCount);
}

Sea::~Sea()
{
	vb_playtile.Clean();
	vb_seatiles.Clean();
}

//Tengert inicializálja
void Sea::Init(int map_size)
{
	InitPlayTiles(map_size);

	InitSeaTiles();
	//RemoveExtraSeaTiles();
	
	seaTileTextureID = GLUtils::TextureFromFile("Resources/Textures/seaTexture.bmp");
}

//Tenger összes mezõjét inicializálja
void Sea::InitSeaTiles()
{
	vb_seatiles.AddAttribute(0, 3); //pozíció
	vb_seatiles.AddAttribute(1, 4); //szín+transparency
	vb_seatiles.AddAttribute(2, 3); //normálvektor
	vb_seatiles.AddAttribute(3, 2); //textúra koord

	float seatile_transX;
	float seatile_transZ;
	bool found;
	int foundCount = 0;

	for (int i = 0; i < seaTileCount; ++i) {
		seatile_transX = ((i % seaTileRow) * PlayTile::getScaleXZ() * TorpedoGLOBAL::Scale)
			- (PlayTile::getScaleXZ() * seaTileRow / 2.0f * TorpedoGLOBAL::Scale) + TorpedoGLOBAL::Scale;//Ide ezkell,hogy extra tileok kijöjjenek
		seatile_transZ = ((i / seaTileRow) * PlayTile::getScaleXZ() * TorpedoGLOBAL::Scale)
			- (PlayTile::getScaleXZ() * seaTileRow / 2.0f * TorpedoGLOBAL::Scale);//Itt nemkell,hogy az extra tileokat ki lehessen szedni

		found = false;
		for (PlayTile pTile : myTiles) {
			if (CompareTileTranslations(glm::vec3(seatile_transX, 0, seatile_transZ), pTile)) {
				found = true;
				break;
			}
		}
		if (!found) {
			for (PlayTile pTile : enemyTiles) {
				if (CompareTileTranslations(glm::vec3(seatile_transX, 0, seatile_transZ), pTile)) {
					found = true;
					break;
				}
			}
		}
		if(found){
			foundCount++;
			continue;
		}

		vb_seatiles.AddData(0, -1.0f + seatile_transX, 0, -1.0f + seatile_transZ);
		vb_seatiles.AddData(0,  1.0f + seatile_transX, 0, -1.0f + seatile_transZ);
		vb_seatiles.AddData(0, -1.0f + seatile_transX, 0,  1.0f + seatile_transZ);
		vb_seatiles.AddData(0,  1.0f + seatile_transX, 0,  1.0f + seatile_transZ);

		vb_seatiles.AddData(1, 0, 0, 1.0f, 0.5f);
		vb_seatiles.AddData(1, 0, 0, 1.0f, 0.5f);
		vb_seatiles.AddData(1, 0, 0, 1.0f, 0.5f);
		vb_seatiles.AddData(1, 0, 0, 1.0f, 0.5f);

		vb_seatiles.AddData(2, 0, 1.0f, 0);
		vb_seatiles.AddData(2, 0, 1.0f, 0);
		vb_seatiles.AddData(2, 0, 1.0f, 0);
		vb_seatiles.AddData(2, 0, 1.0f, 0);
				  
		vb_seatiles.AddData(3, 0, 0);
		vb_seatiles.AddData(3, 1, 0);
		vb_seatiles.AddData(3, 0, 1);
		vb_seatiles.AddData(3, 1, 1);

		int t = i - foundCount;
		vb_seatiles.AddIndex((t * 4) + 1, (t * 4) + 0, (t * 4) + 2);
		vb_seatiles.AddIndex((t * 4) + 1, (t * 4) + 2, (t * 4) + 3);

	}

	vb_seatiles.InitBuffers();
	
	//glm::vec3 seatileResult = glm::vec3(0.0f);
	//for (int i = 0; i < seaTileCount; ++i) {
	//	seatile_transX = ((i % seaTileScale) * SeaTile::getScaleXZ() * TorpedoGLOBAL::Scale)
	//		- (SeaTile::getScaleXZ() * seaTileScale / 2.0f * TorpedoGLOBAL::Scale) + TorpedoGLOBAL::Scale;//Ide ezkell,hogy extra tileok kijöjjenek
	//	seatile_transZ = ((i / seaTileScale) * SeaTile::getScaleXZ() * TorpedoGLOBAL::Scale)
	//		- (SeaTile::getScaleXZ() * seaTileScale / 2.0f * TorpedoGLOBAL::Scale);//Itt nemkell,hogy az extra tileokat ki lehessen szedni
	//
	//	seatileResult += glm::vec3(seatile_transX, 0, seatile_transZ);
	//
	//	seaTiles.push_back(new SeaTile(seatileResult));
	//
	//	seatileResult = glm::vec3(0.0f);
	//}
}

//Játékmezõket inicializálja - mapSize*mapSize, enemyjé offsetelve 100al
void Sea::InitPlayTiles(int map_size)
{
	vb_playtile.AddAttribute(0, 3); //pozíció
	vb_playtile.AddAttribute(1, 4); //szín+transparency
	vb_playtile.AddAttribute(2, 3); //normálvektor
	vb_playtile.AddAttribute(3, 2); //textúra koord

	vb_playtile.AddData(0, -0.5f, 0, -0.5f);
	vb_playtile.AddData(0, 0.5f, 0, -0.5f);
	vb_playtile.AddData(0, -0.5f, 0, 0.5f);
	vb_playtile.AddData(0, 0.5f, 0, 0.5f);

	vb_playtile.AddData(1, 0, 0, 1.0f, 0.5f);
	vb_playtile.AddData(1, 0, 0, 1.0f, 0.5f);
	vb_playtile.AddData(1, 0, 0, 1.0f, 0.5f);
	vb_playtile.AddData(1, 0, 0, 1.0f, 0.5f);

	vb_playtile.AddData(2, 0, 1.0f, 0);
	vb_playtile.AddData(2, 0, 1.0f, 0);
	vb_playtile.AddData(2, 0, 1.0f, 0);
	vb_playtile.AddData(2, 0, 1.0f, 0);

	vb_playtile.AddData(3, 0, 0);
	vb_playtile.AddData(3, 1, 0);
	vb_playtile.AddData(3, 0, 1);
	vb_playtile.AddData(3, 1, 1);

	vb_playtile.AddIndex(1, 0, 2);
	vb_playtile.AddIndex(1, 2, 3);

	vb_playtile.InitBuffers();

	mapSize = map_size;
	playTileCount = mapSize*mapSize;
	myTiles.reserve(playTileCount);
	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++) {
			myTiles.push_back(PlayTile(std::pair<char, int>(static_cast<char>('a' + i), 1 + j),
				calcTranslate(i, j, true)));
			//myTiles[i*mapSize + j].setTranslate(calcTranslate(i, j, true));
			myTiles[i*mapSize + j].setIndex(i*mapSize + j);
		}
	}

	enemyTiles.reserve(playTileCount);
	for (int i = 0; i < mapSize; i++) {
		for (int j = 0; j < mapSize; j++) {
			enemyTiles.push_back(PlayTile(std::pair<char, int>(static_cast<char>('a' + i), 1 + j),
				calcTranslate(i, j, false)));
			//enemyTiles[i*mapSize + j].setTranslate(calcTranslate(i, j, false));
			enemyTiles[i*mapSize + j].setIndex(i*mapSize + j + enemyTilesIndexOffset);
		}
	}
}

//Kiszámolja hogy hova kell elmozgatni egy játékmezõt grafikailag
glm::vec3 Sea::calcTranslate(int row_nr, int column_nr, bool ally) const
{
	float tile_transX = 0;
	float tile_transZ = 0;
	glm::vec3 tileResult = glm::vec3(0.0f);
	tile_transX = column_nr * PlayTile::getScaleXZ() * TorpedoGLOBAL::Scale;
	tile_transZ = (row_nr * PlayTile::getScaleXZ() * TorpedoGLOBAL::Scale)
		- ((mapSize - 1) * PlayTile::getScaleXZ() / 2.0f * TorpedoGLOBAL::Scale);

	tileResult += glm::vec3(mountaincenterBorderXOffset, 0, 0) +
		glm::vec3(mountainTileOffset, 0, 0) +
		glm::vec3(tile_transX, 0, tile_transZ);

	if (ally) {
		tileResult = glm::vec3(-1, 1, 1)*tileResult;
	}
	else {
		tileResult = glm::vec3(1, 1, -1)*tileResult;
	}
	return tileResult;
}


//Tenger animációhoz kellõ számítások
void Sea::Update(float delta_time)
{
	textureAnimationOffset += delta_time;
	//std::cout << textureAnimationOffset << std::endl;
}

//Egy elõrajzolás,ami szükséges hogy tudjuk melyik játékmezõre mutatunk az egérrel
void Sea::PreProcess(const gCamera& camera, gShaderProgram& sh_program) const
{
	glDisable(GL_CULL_FACE);
	sh_program.SetUniform("is_preprocess", true);
	for (int i = 0; i < (mapSize*mapSize); i++) {
		enemyTiles[i].PreProcess(camera, sh_program, vb_playtile);
		myTiles[i].PreProcess(camera, sh_program, vb_playtile);
	}
	glEnable(GL_CULL_FACE);
}

//Kirajzolja a játékmezõket
void Sea::Draw(const gCamera& camera, gShaderProgram& sh_program) const
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glStencilMask(0xFF);
	//sh_program.SetUniform("read_index", static_cast<int>(pointedTile));
	sh_program.SetUniform("seatileOffset", textureAnimationOffset);

	sh_program.SetUniform("is_seatile", true);
	sh_program.SetUniform("hasTexture", true);
	sh_program.SetTexture("texImage", 0, seaTileTextureID);

	//std::cout << myTiles[1].getOutlineWidth() << " index " << myTiles[1].getIndex() << std::endl;
	for (int i = 0; i < (mapSize*mapSize); i++) {
		enemyTiles[i].Draw(camera, sh_program, vb_playtile);
		myTiles[i].Draw(camera, sh_program, vb_playtile);
	}
	glStencilMask(0x00);
	//for (SeaTile* tile : seaTiles) {
		//tile->Draw(camera, sh_program);
	//}

	//glm::mat4 matWorld = glm::mat4(1.0f);
	//glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() * matWorld;
	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);

	vb_seatiles.On();
	vb_seatiles.DrawIndexed(GL_TRIANGLES, 0, 6 * seaTileCount, 0);
	vb_seatiles.Off();
	sh_program.SetUniform("is_seatile", false);
	sh_program.SetUniform("hasTexture", false);

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

//Mindegyik játékmezõre meghívja a körvonal rajzolást
void Sea::OutlineDraw(const gCamera& camera, gShaderProgram& sh_program, float pointed_tile) const
{
	glDisable(GL_CULL_FACE);
	sh_program.SetUniform("read_index", static_cast<int>(pointed_tile));
	//std::cout << pointedTile << std::endl;
	sh_program.SetUniform("is_preprocess", false);
	for (int i = 0; i < (mapSize*mapSize); i++) {
		enemyTiles[i].OutlineDraw(camera, sh_program, vb_playtile);
		myTiles[i].OutlineDraw(camera, sh_program, vb_playtile);
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
int Sea::getEnemyIndexOffset() const
{
	return enemyTilesIndexOffset;
}

//Kiszedi a azokat az extra tengermezõket,amik a játékmezõk helyén vannak
//void Sea::RemoveExtraSeaTiles()
//{
//	int initialSize = seaTiles.size();
//	//bool foundInMyTiles;
//	for (std::vector<SeaTile*>::iterator it = seaTiles.begin(); it != seaTiles.end();++it) 
//	{
//		//foundInMyTiles = false;
//		for (PlayTile pTile : myTiles) {
//			if (CompareTileTranslations(*it,pTile)) {
//				seaTiles.erase(it);
//				--it;
//				break;
//			}
//		}
//		//if (foundInMyTiles) {
//		//	foundInMyTiles = false;
//		//	continue;
//		//}
//		for (PlayTile pTile : enemyTiles) {
//			if (CompareTileTranslations(*it, pTile)) {
//				seaTiles.erase(it);
//				--it;
//				break;
//			}
//		}
//	}
//	if (initialSize - 2 * myTiles.size() != seaTiles.size()) {
//		std::cout << "[Sea::RemoveExtraSeaTiles]: Extra SeaTiles were not removed correctly! initial: " 
//			<< initialSize << " after: " << seaTiles.size() << std::endl;
//	}
//}

//Két játékmezõ elmozgatását nézi meg hogy float közelítõleg egyenlõ-e
bool Sea::CompareTileTranslations(const glm::vec3 seatile_trans, const PlayTile &p_tile) const
{
	return (round(seatile_trans.x) == round(p_tile.getTranslate().x)
		&& round(seatile_trans.z) == round(p_tile.getTranslate().z));
}



