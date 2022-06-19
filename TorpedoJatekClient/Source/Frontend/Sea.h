#pragma once

#include "PlayTile.h"
#include "Mountain.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"
#include "../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Tenger osztálya-2játékos játékmezõi+többi tengermezõ
class Sea
{
public:
	Sea(void);
	~Sea(void);

	void Init(int inMapSize = 7);
	void Update(float deltatime);
	void PreProcess(gCamera& camera, gShaderProgram& sh_program);
	void Draw(gCamera& camera, gShaderProgram& sh_program);
	void OutlineDraw(gCamera& camera, gShaderProgram& sh_program, float pointedTile);

	std::vector<PlayTile>& getTiles(bool ally = true);
	PlayTile& getTileByIndex(int index);

	int getEnemyIndexOffset();
protected:
	void InitSeaTiles();
	void InitPlayTiles(int inMapSize);
	void RemoveExtraSeaTiles();
	bool CompareTileTranslations(SeaTile &sTile, PlayTile &pTile);
	glm::vec3 calcTranslate(int rowNr, int columnNr, bool ally = true) const;

	GLuint seaTileTextureID; //Tengerdarab textúra azonosítója
	const int seaTileScale = 50 * 2; //hány tengermezõ van egy sorba
	const int seaTileCount = seaTileScale * seaTileScale; //hány tengermezõ van összesen
	std::vector<SeaTile> seaTiles; //Tengermezõket tartalmazza

	int mapSize; //játékPálya mérete
	int playTileCount;	//játékMezõk száma
	std::vector<PlayTile> myTiles;	//kliens játékmezõi
	std::vector<PlayTile> enemyTiles; //ellenfél játékmezõi
	int enemyTilesIndexOffset = 100; //offsetelni az ellenfélmezõ indexeket
	float textureAnimationOffset = 0.0f; //jelenleg mennyire mozgatjuk el a textúrát

	const float mountain_tile_offset = (2 * SeaTile::getScaleXZ())*TorpedoGLOBAL::Scale; //hegy és elsõ játékmezõ közti táv
	const float mountaincenter_border_Xoffset = Mountain::getWidthX() / 2.0f*TorpedoGLOBAL::Scale; //hegy közepe és széle közti táv

};
