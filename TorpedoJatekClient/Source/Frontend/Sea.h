#pragma once

#include "PlayTile.h"

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

	void Init(int map_size, float pt_center_offset);
	void Update(float delta_time);
	void PreProcess(const gCamera& camera, gShaderProgram& sh_program) const;
	void Draw(const gCamera& camera, gShaderProgram& sh_program) const;
	void OutlineDraw(const gCamera& camera, gShaderProgram& sh_program, float pointed_tile) const;

	std::vector<PlayTile*>& getTiles(bool ally = true);
	PlayTile& getTileByIndex(int index, bool ally = true);

	int getAlphaOffset() const;
	int getEnemyIndexOffset() const;
	int getSeaTileRow() const;
private:
	void InitPlayTiles();
	void InitSeaTiles();
	bool CompareTileTranslations(const glm::vec3 seatile_trans, const PlayTile& p_tile) const;
	const glm::vec3 calcTranslate(int row_nr, int column_nr, bool ally = true) const;

	gVertexBuffer vb_seatiles; //összes tengermezõ grafikai modell adatok
	gVertexBuffer vb_playtile; //egy játékmezõ modell adatok
	GLuint seaTileTextureID; //Tengerdarab textúra azonosítója
	std::vector<PlayTile*> myTiles;	//kliens játékmezõi
	std::vector<PlayTile*> enemyTiles; //ellenfél játékmezõi

	const int seaTileRow = 50 * 2; //hány tengermezõ van egy sorba
	const int seaTileCount = seaTileRow * seaTileRow; //hány tengermezõ van összesen
	const int enemyTilesOffset = 100; //offsetelni az ellenfélmezõ indexeket
	const int alphaOffset = 100; //offsetelni az indexet,hogy ne 0-1 legyen AMÉG ALPHA színbe írom az indexet
	const glm::vec3 stScale = glm::vec3(TorpedoGLOBAL::SeaTileScaleXZ, 1.0f, TorpedoGLOBAL::SeaTileScaleXZ) * TorpedoGLOBAL::Scale;

	int mapSize; //játékPálya mérete
	int playTileCount;	//játékMezõk száma
	float textureAnimationOffset = 0.0f; //jelenleg mennyire mozgatjuk el a textúrát
	float ptCenterOffset; //Játékmezõk origótól való távolsága

	const glm::mat4 matWorld; //SeaTile világ transzformáció
	const glm::mat4 matWorldIT; //VT inverze
};
