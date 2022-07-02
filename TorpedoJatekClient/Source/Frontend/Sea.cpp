#include "Sea.h"

//Itt a matWorld a SeaTileok VB-j�be �getett transzok ut�n csin�lna vlmit,ugyh ink�bb hagyjuk igy
Sea::Sea() : matWorld(glm::mat4(1.0f)),
	matWorldIT(glm::transpose(glm::inverse(matWorld)))
{
}

Sea::~Sea()
{
	vb_playtile.Clean();
	vb_seatiles.Clean();
	for (PlayTile* &tile : myTiles) {
		delete tile;
	}
	for (PlayTile* &tile : enemyTiles) {
		delete tile;
	}
}

//Tengert inicializ�lja
void Sea::Init(int map_size, float pt_center_offset)
{
	mapSize = map_size;
	ptCenterOffset = pt_center_offset;

	InitPlayTiles();
	InitSeaTiles();
	
	seaTileTextureID = GLUtils::TextureFromFile("Resources/Textures/seaTexture.bmp");
}

//Tenger �sszes mez�j�t inicializ�lja
void Sea::InitSeaTiles()
{
	vb_seatiles.AddAttribute(0, 3); //poz�ci�
	vb_seatiles.AddAttribute(1, 4); //sz�n+transparency
	vb_seatiles.AddAttribute(2, 3); //norm�lvektor
	vb_seatiles.AddAttribute(3, 2); //text�ra koord

	float seatile_transX;
	float seatile_transZ;
	bool found;
	int foundCount = 0;

	for (int i = 0; i < seaTileCount; ++i) {
		seatile_transX = ((i % seaTileRow) * TorpedoGLOBAL::SeaTileScaleXZ * TorpedoGLOBAL::Scale)
			//Lefelezz�k �s kivonjuk szimmetria miatt
			- (seaTileRow * TorpedoGLOBAL::SeaTileScaleXZ / 2.0f * TorpedoGLOBAL::Scale);
		seatile_transZ = ((i / seaTileRow) * TorpedoGLOBAL::SeaTileScaleXZ * TorpedoGLOBAL::Scale)
			- (seaTileRow * TorpedoGLOBAL::SeaTileScaleXZ / 2.0f * TorpedoGLOBAL::Scale);

		//if (i == 0) {
		//	std::cout << "Seatile: " << seatile_transX << " " << seatile_transZ << std::endl;
		//}

		//Kiszedi a PlayTile-al azonos helyen l�v�ket
		found = false;
		glm::vec3 trans = glm::vec3(seatile_transX, 0, seatile_transZ);
		for (const PlayTile* pTile : myTiles) {
			if (CompareTileTranslations(trans, *pTile)) {
				found = true;
				break;
			}
		}
		if (!found) {
			for (const PlayTile* pTile : enemyTiles) {
				if (CompareTileTranslations(trans, *pTile)) {
					found = true;
					break;
				}
			}
		}
		if(found){
			foundCount++;
			continue;
		}

		vb_seatiles.AddData(0, -0.5f * stScale.x + seatile_transX, 0, -0.5f * stScale.z + seatile_transZ);
		vb_seatiles.AddData(0,  0.5f * stScale.x + seatile_transX, 0, -0.5f * stScale.z + seatile_transZ);
		vb_seatiles.AddData(0, -0.5f * stScale.x + seatile_transX, 0,  0.5f * stScale.z + seatile_transZ);
		vb_seatiles.AddData(0,  0.5f * stScale.x + seatile_transX, 0,  0.5f * stScale.z + seatile_transZ);

		vb_seatiles.AddData(1, 0, 0, 1.0f, 0.5f);
		vb_seatiles.AddData(1, 0, 0, 1.0f, 0.5f);
		vb_seatiles.AddData(1, 0, 0, 1.0f, 0.5f);
		vb_seatiles.AddData(1, 0, 0, 1.0f, 0.5f);

		vb_seatiles.AddData(2, 0, 1.0f, 0);
		vb_seatiles.AddData(2, 0, 1.0f, 0);
		vb_seatiles.AddData(2, 0, 1.0f, 0);
		vb_seatiles.AddData(2, 0, 1.0f, 0);
				  
		vb_seatiles.AddData(3, 0, 1);
		vb_seatiles.AddData(3, 1, 1);
		vb_seatiles.AddData(3, 0, 0);
		vb_seatiles.AddData(3, 1, 0);

		int t = i - foundCount;
		vb_seatiles.AddIndex((t * 4) + 1, (t * 4) + 0, (t * 4) + 2);
		vb_seatiles.AddIndex((t * 4) + 1, (t * 4) + 2, (t * 4) + 3);

	}

	//2 j�t�kos van
	if (foundCount != playTileCount * 2) {
		std::cout << "[Init_SeaTiles] Error while removing extra SeaTiles: " << foundCount
			<< " != " << playTileCount << std::endl;
	}

	vb_seatiles.InitBuffers();
}

//J�t�kmez�ket inicializ�lja - mapSize*mapSize, enemyj� offsetelve 100al
void Sea::InitPlayTiles()
{
	vb_playtile.AddAttribute(0, 3); //poz�ci�
	vb_playtile.AddAttribute(1, 4); //sz�n+transparency
	vb_playtile.AddAttribute(2, 3); //norm�lvektor
	vb_playtile.AddAttribute(3, 2); //text�ra koord

	vb_playtile.AddData(0, -0.5f, 0, -0.5f);
	vb_playtile.AddData(0,  0.5f, 0, -0.5f);
	vb_playtile.AddData(0, -0.5f, 0,  0.5f);
	vb_playtile.AddData(0,  0.5f, 0,  0.5f);

	vb_playtile.AddData(1, 0, 0, 1.0f, 0.5f);
	vb_playtile.AddData(1, 0, 0, 1.0f, 0.5f);
	vb_playtile.AddData(1, 0, 0, 1.0f, 0.5f);
	vb_playtile.AddData(1, 0, 0, 1.0f, 0.5f);

	vb_playtile.AddData(2, 0, 1.0f, 0);
	vb_playtile.AddData(2, 0, 1.0f, 0);
	vb_playtile.AddData(2, 0, 1.0f, 0);
	vb_playtile.AddData(2, 0, 1.0f, 0);

	vb_playtile.AddData(3, 0, 1);
	vb_playtile.AddData(3, 1, 1);
	vb_playtile.AddData(3, 0, 0);
	vb_playtile.AddData(3, 1, 0);

	vb_playtile.AddIndex(1, 0, 2);
	vb_playtile.AddIndex(1, 2, 3);

	vb_playtile.InitBuffers();

	playTileCount = mapSize * mapSize;
	int ind;
	myTiles.reserve(playTileCount);
	enemyTiles.reserve(playTileCount);
	//std::cout << "Playtile: " << calcTranslate(6, 0, true).x << " " << calcTranslate(6, 0, true).z << std::endl;
	for (int i = 0; i < mapSize; ++i) {
		for (int j = 0; j < mapSize; ++j) {
			ind = i * mapSize + j;
			myTiles.push_back(new PlayTile(std::pair<char, int>(static_cast<char>('a' + i), 1 + j),
				calcTranslate(i, j, true), stScale, ind, ind + alphaOffset));
			enemyTiles.push_back(new PlayTile(std::pair<char, int>(static_cast<char>('a' + i), 1 + j),
				calcTranslate(i, j, false), stScale, ind, ind + alphaOffset + enemyTilesOffset));
		}
	}
}

//Kisz�molja hogy hova kell elmozgatni egy j�t�kmez�t grafikailag
const glm::vec3 Sea::calcTranslate(int column_nr, int row_nr, bool ally) const
{
	float tile_transX = row_nr * TorpedoGLOBAL::SeaTileScaleXZ * TorpedoGLOBAL::Scale;
	float tile_transZ = (column_nr * TorpedoGLOBAL::SeaTileScaleXZ * TorpedoGLOBAL::Scale)
		//Levonjuk hogy szimmetrikus legyen Z tengelyre
		- ((mapSize - 1) * TorpedoGLOBAL::SeaTileScaleXZ / 2.0f * TorpedoGLOBAL::Scale);

	glm::vec3 tileResult = glm::vec3(ptCenterOffset + tile_transX,
		0, tile_transZ); 

	if (ally) {
		//Balra toljuk player t�rfel�re
		tileResult *= glm::vec3(-1, 1, 1); 
	}
	else {
		//�tford�tjuk hogy ellenf�lnek is balr�l-jobbra legyenek a mez�k
		tileResult *= glm::vec3(1, 1, -1); 
	}
	return tileResult;
}


//Tenger anim�ci�hoz kell� sz�m�t�sok
void Sea::Update(float delta_time)
{
	textureAnimationOffset += delta_time;
}

//Egy el�rajzol�s,ami sz�ks�ges hogy tudjuk melyik j�t�kmez�re mutatunk az eg�rrel
void Sea::PreProcess(const gCamera& camera, gShaderProgram& sh_program) const
{
	glDisable(GL_CULL_FACE);
	sh_program.SetUniform("is_preprocess", true);
	for (int i = 0; i < (mapSize*mapSize); ++i) {
		enemyTiles.at(i)->PreProcess(camera, sh_program, vb_playtile);
		myTiles.at(i)->PreProcess(camera, sh_program, vb_playtile);
	}
	glEnable(GL_CULL_FACE);
}

//Kirajzolja a j�t�kmez�ket
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

	for (int i = 0; i < (mapSize*mapSize); ++i) {
		enemyTiles.at(i)->Draw(camera, sh_program, vb_playtile);
		myTiles.at(i)->Draw(camera, sh_program, vb_playtile);
	}
	glStencilMask(0x00);

	glm::mat4 mvp = camera.GetViewProj() * matWorld;
	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);

	vb_seatiles.On();
	vb_seatiles.DrawIndexed(GL_TRIANGLES, 0, 6 * seaTileCount);
	vb_seatiles.Off();
	sh_program.SetUniform("is_seatile", false);
	sh_program.SetUniform("hasTexture", false);

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

//Mindegyik j�t�kmez�re megh�vja a k�rvonal rajzol�st
void Sea::OutlineDraw(const gCamera& camera, gShaderProgram& sh_program, float pointed_tile) const
{
	glDisable(GL_CULL_FACE);
	sh_program.SetUniform("read_index", static_cast<int>(pointed_tile));
	sh_program.SetUniform("is_preprocess", false);
	for (int i = 0; i < (mapSize*mapSize); ++i) {
		enemyTiles.at(i)->OutlineDraw(camera, sh_program, vb_playtile);
		myTiles.at(i)->OutlineDraw(camera, sh_program, vb_playtile);
	}
	glEnable(GL_CULL_FACE);
}

//Visszaadja egy j�t�kos j�t�kmez�it
std::vector<PlayTile*>& Sea::getTiles(bool ally)
{
	if (ally) {
		return myTiles;
	}
	else {
		return enemyTiles;
	}
}

//J�t�kmez� indexe alapj�n adja vissza a j�t�kmez�t
PlayTile& Sea::getTileByIndex(int index, bool ally)
{
	if (ally) {
		for (PlayTile* &tile : myTiles) {
			if (tile->getIndex() == index) {
				return *tile;
			}
		}
	}
	else {
		for (PlayTile* &tile : enemyTiles) {
			if (tile->getIndex() == index) {
				return *tile;
			}
		}
	}
	
}

//Az ellenf�l j�t�kmez�indexei el vannak tolva,hogy mikor kiolvassuk akkor lehessen tudni
int Sea::getEnemyIndexOffset() const
{
	return enemyTilesOffset;
}

//K�t j�t�kmez� elmozgat�s�t n�zi meg hogy float k�zel�t�leg egyenl�-e
bool Sea::CompareTileTranslations(const glm::vec3 seatile_trans, const PlayTile &p_tile) const
{
	return (round(seatile_trans.x) == round(p_tile.getTranslate().x)
		&& round(seatile_trans.z) == round(p_tile.getTranslate().z));
}

int Sea::getAlphaOffset() const
{
	return alphaOffset;
}

int Sea::getSeaTileRow() const
{
	return seaTileRow;
}
