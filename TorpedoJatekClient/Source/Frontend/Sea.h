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
	Sea();
	~Sea();

	void Init(int map_size = 7);
	void Update(float delta_time);
	void PreProcess(const gCamera& camera, gShaderProgram& sh_program) const;
	void Draw(const gCamera& camera, gShaderProgram& sh_program) const;
	void OutlineDraw(const gCamera& camera, gShaderProgram& sh_program, float pointed_tile) const;

	std::vector<PlayTile>& getTiles(bool ally = true);
	PlayTile& getTileByIndex(int index);

	int getEnemyIndexOffset() const;
protected:
	void InitSeaTiles();
	void InitPlayTiles(int map_size);
	//void RemoveExtraSeaTiles();
	bool CompareTileTranslations(const glm::vec3 seatile_trans, const PlayTile& p_tile) const;
	glm::vec3 calcTranslate(int row_nr, int column_nr, bool ally = true) const;

	gVertexBuffer vb_seatiles; //összes tengermezõ grafikai modell adatok
	gVertexBuffer vb_playtile; //egy játékmezõ modell adatok
	GLuint seaTileTextureID; //Tengerdarab textúra azonosítója

	const int seaTileRow = 50 * 2; //hány tengermezõ van egy sorba
	const int seaTileCount = seaTileRow * seaTileRow; //hány tengermezõ van összesen
	//std::vector<SeaTile*> seaTiles; //Tengermezõket tartalmazza

	int mapSize; //játékPálya mérete
	int playTileCount;	//játékMezõk száma
	std::vector<PlayTile> myTiles;	//kliens játékmezõi
	std::vector<PlayTile> enemyTiles; //ellenfél játékmezõi
	const int enemyTilesIndexOffset = 100; //offsetelni az ellenfélmezõ indexeket
	float textureAnimationOffset = 0.0f; //jelenleg mennyire mozgatjuk el a textúrát

	const float mountainTileOffset = (2.0f * PlayTile::getScaleXZ()) * TorpedoGLOBAL::Scale; //hegy és elsõ játékmezõ közti táv
	const float mountaincenterBorderXOffset = Mountain::getWidthX() / 2.0f * TorpedoGLOBAL::Scale; //hegy közepe és széle közti táv
	//glm::vec3 seaTileScale = glm::vec3(tileScaleXZ, 1.0f, tileScaleXZ) * TorpedoGLOBAL::Scale; //nagyítás

	//static const float tileScaleXZ; //tengermezõ skálázása XZ tengelyek mentén
	const glm::mat4 matWorld; //SeaTile világ transzformáció
	const glm::mat4 matWorldIT; //VT inverze
};
