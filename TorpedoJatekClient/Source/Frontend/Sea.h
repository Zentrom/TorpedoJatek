#pragma once

#include "PlayTile.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"
#include "../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Tenger oszt�lya-2j�t�kos j�t�kmez�i+t�bbi tengermez�
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

	gVertexBuffer vb_seatiles; //�sszes tengermez� grafikai modell adatok
	gVertexBuffer vb_playtile; //egy j�t�kmez� modell adatok
	GLuint seaTileTextureID; //Tengerdarab text�ra azonos�t�ja
	std::vector<PlayTile*> myTiles;	//kliens j�t�kmez�i
	std::vector<PlayTile*> enemyTiles; //ellenf�l j�t�kmez�i

	const int seaTileRow = 50 * 2; //h�ny tengermez� van egy sorba
	const int seaTileCount = seaTileRow * seaTileRow; //h�ny tengermez� van �sszesen
	const int enemyTilesOffset = 100; //offsetelni az ellenf�lmez� indexeket
	const int alphaOffset = 100; //offsetelni az indexet,hogy ne 0-1 legyen AM�G ALPHA sz�nbe �rom az indexet
	const glm::vec3 stScale = glm::vec3(TorpedoGLOBAL::SeaTileScaleXZ, 1.0f, TorpedoGLOBAL::SeaTileScaleXZ) * TorpedoGLOBAL::Scale;

	int mapSize; //j�t�kP�lya m�rete
	int playTileCount;	//j�t�kMez�k sz�ma
	float textureAnimationOffset = 0.0f; //jelenleg mennyire mozgatjuk el a text�r�t
	float ptCenterOffset; //J�t�kmez�k orig�t�l val� t�vols�ga

	const glm::mat4 matWorld; //SeaTile vil�g transzform�ci�
	const glm::mat4 matWorldIT; //VT inverze
};
