#include "Sea.h"

//Itt a matWorld a SeaTileok VB-jébe égetett transzok után csinálna vlmit,ugyh inkább hagyjuk igy
Sea::Sea() : matWorld(glm::mat4(1.0f)),
	matWorldIT(glm::transpose(glm::inverse(matWorld)))
{
}

Sea::~Sea()
{
	vb_playtile.Clean();
	vb_seatiles.Clean();
	sh_ptOutline.Clean();
	sh_seaTile.Clean();

	for (PlayTile* &tile : myTiles) {
		delete tile;
	}
	for (PlayTile* &tile : enemyTiles) {
		delete tile;
	}
}

//Tengert inicializálja
void Sea::Init(int map_size, float pt_center_offset)
{
	mapSize = map_size;
	ptCenterOffset = pt_center_offset;

	InitPlayTiles();
	InitSeaTiles();
	
	sh_ptOutline.AttachShader(GL_VERTEX_SHADER, "Shaders/ptOutline.vert");
	sh_ptOutline.AttachShader(GL_FRAGMENT_SHADER, "Shaders/ptOutline.frag");
	sh_ptOutline.BindAttribLoc(0, "vs_in_pos");
	sh_ptOutline.BindAttribLoc(1, "vs_in_col");
	if (!sh_ptOutline.LinkProgram()) {
		std::cout << "[Shader_Link]Error during Shader compilation: sh_ptOutline" << std::endl;
	}

	sh_seaTile.AttachShader(GL_VERTEX_SHADER, "Shaders/seatile.vert");
	sh_seaTile.AttachShader(GL_FRAGMENT_SHADER, "Shaders/seatile.frag");
	sh_seaTile.BindAttribLoc(0, "vs_in_pos");
	sh_seaTile.BindAttribLoc(1, "vs_in_color");
	sh_seaTile.BindAttribLoc(2, "vs_in_normal");
	sh_seaTile.BindAttribLoc(3, "vs_in_tex");
	if (!sh_seaTile.LinkProgram())
	{
		std::cout << "[Shader_Link]Error during Shader compilation: sh_seaTile" << std::endl;
	}

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
		seatile_transX = ((i % seaTileRow) * TorpedoGLOBAL::SeaTileScaleXZ * TorpedoGLOBAL::Scale)
			//Lefelezzük és kivonjuk szimmetria miatt
			- (seaTileRow * TorpedoGLOBAL::SeaTileScaleXZ / 2.0f * TorpedoGLOBAL::Scale);
		seatile_transZ = ((i / seaTileRow) * TorpedoGLOBAL::SeaTileScaleXZ * TorpedoGLOBAL::Scale)
			- (seaTileRow * TorpedoGLOBAL::SeaTileScaleXZ / 2.0f * TorpedoGLOBAL::Scale);

		//Kiszedi a PlayTile-al azonos helyen lévõket
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

	//2 játékos van
	if (foundCount != playTileCount * 2) {
		std::cout << "[Init_SeaTiles] Error while removing extra SeaTiles: " << foundCount
			<< " != " << playTileCount << std::endl;
	}

	vb_seatiles.InitBuffers();
}

//Játékmezõket inicializálja - mapSize*mapSize, enemyjé offsetelve 100al
void Sea::InitPlayTiles()
{
	vb_playtile.AddAttribute(0, 3); //pozíció
	vb_playtile.AddAttribute(1, 4); //szín+transparency
	vb_playtile.AddAttribute(2, 3); //normálvektor
	vb_playtile.AddAttribute(3, 2); //textúra koord

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

//Kiszámolja hogy hova kell elmozgatni egy játékmezõt grafikailag
const glm::vec3 Sea::calcTranslate(int column_nr, int row_nr, bool ally) const
{
	float tile_transX = row_nr * TorpedoGLOBAL::SeaTileScaleXZ * TorpedoGLOBAL::Scale;
	float tile_transZ = (column_nr * TorpedoGLOBAL::SeaTileScaleXZ * TorpedoGLOBAL::Scale)
		//Levonjuk hogy szimmetrikus legyen Z tengelyre
		- ((mapSize - 1) * TorpedoGLOBAL::SeaTileScaleXZ / 2.0f * TorpedoGLOBAL::Scale);

	glm::vec3 tileResult = glm::vec3(ptCenterOffset + tile_transX,
		0, tile_transZ); 

	if (ally) {
		//Balra toljuk player térfelére
		tileResult *= glm::vec3(-1, 1, 1); 
	}
	else {
		//Átfordítjuk hogy ellenfélnek is balról-jobbra legyenek a mezõk
		tileResult *= glm::vec3(1, 1, -1); 
	}
	return tileResult;
}


//Tenger animációhoz kellõ számítások
void Sea::Update(float delta_time)
{
	textureAnimationOffset += delta_time;
}

//Egy elõrajzolás,ami szükséges hogy tudjuk melyik játékmezõre mutatunk az egérrel
void Sea::PreProcess(const gCamera& camera)
{
	glDisable(GL_CULL_FACE);

	sh_ptOutline.On();
	sh_ptOutline.SetUniform("is_preprocess", true);
	for (int i = 0; i < (mapSize*mapSize); ++i) {
		enemyTiles.at(i)->PreProcess(camera, sh_ptOutline, vb_playtile);
		myTiles.at(i)->PreProcess(camera, sh_ptOutline, vb_playtile);
	}
	sh_ptOutline.Off();

	glEnable(GL_CULL_FACE);
}

//Kirajzolja a játékmezõket
void Sea::Draw(const gCamera& camera)
{
	glStencilMask(0xFF);

	sh_seaTile.On();
	sh_seaTile.SetUniform("seatileTexOffset", textureAnimationOffset);
	sh_seaTile.SetUniform("hasTexture", true);
	sh_seaTile.SetTexture("texImage", 0, seaTileTextureID);

	for (int i = 0; i < (mapSize*mapSize); ++i) {
		enemyTiles.at(i)->Draw(camera, sh_seaTile, vb_playtile);
		myTiles.at(i)->Draw(camera, sh_seaTile, vb_playtile);
	}
	glStencilMask(0x00);

	glm::mat4 mvp = camera.GetViewProj() * matWorld;
	sh_seaTile.SetUniform("world", matWorld);
	sh_seaTile.SetUniform("worldIT", matWorldIT);
	sh_seaTile.SetUniform("MVP", mvp);

	sh_seaTile.SetUniform("near", camera.getNear());
	sh_seaTile.SetUniform("far", camera.getFar());

	vb_seatiles.On();
	vb_seatiles.DrawIndexed(GL_TRIANGLES, 0, 6 * seaTileCount);
	vb_seatiles.Off();
	//sh_seaTile.SetUniform("hasTexture", false);
	sh_seaTile.Off();
}

//Mindegyik játékmezõre meghívja a körvonal rajzolást
void Sea::OutlineDraw(const gCamera& camera, float pointed_tile)
{
	glDisable(GL_CULL_FACE);

	sh_ptOutline.On();
	sh_ptOutline.SetUniform("read_index", static_cast<int>(pointed_tile));
	sh_ptOutline.SetUniform("is_preprocess", false);
	for (int i = 0; i < (mapSize*mapSize); ++i) {
		enemyTiles.at(i)->OutlineDraw(camera, sh_ptOutline, vb_playtile);
		myTiles.at(i)->OutlineDraw(camera, sh_ptOutline, vb_playtile);
	}
	sh_ptOutline.Off();

	glEnable(GL_CULL_FACE);
}

//Visszaadja egy játékos játékmezõit
std::vector<PlayTile*>& Sea::getTiles(bool ally)
{
	if (ally) {
		return myTiles;
	}
	else {
		return enemyTiles;
	}
}

//Játékmezõ indexe alapján adja vissza a játékmezõt
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
	std::cout << "[Sea_getTileByIndex] No tile found to return! Returning myTiles[0]!" << std::endl;
	return *myTiles.at(0);
}

//Az ellenfél játékmezõindexei el vannak tolva,hogy mikor kiolvassuk akkor lehessen tudni
int Sea::getEnemyIndexOffset() const
{
	return enemyTilesOffset;
}

//Két játékmezõ elmozgatását nézi meg hogy float közelítõleg egyenlõ-e
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
