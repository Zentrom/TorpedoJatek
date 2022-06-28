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

//Tenger oszt�lya-2j�t�kos j�t�kmez�i+t�bbi tengermez�
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

	gVertexBuffer vb_seatiles; //�sszes tengermez� grafikai modell adatok
	gVertexBuffer vb_playtile; //egy j�t�kmez� modell adatok
	GLuint seaTileTextureID; //Tengerdarab text�ra azonos�t�ja

	const int seaTileRow = 50 * 2; //h�ny tengermez� van egy sorba
	const int seaTileCount = seaTileRow * seaTileRow; //h�ny tengermez� van �sszesen
	//std::vector<SeaTile*> seaTiles; //Tengermez�ket tartalmazza

	int mapSize; //j�t�kP�lya m�rete
	int playTileCount;	//j�t�kMez�k sz�ma
	std::vector<PlayTile> myTiles;	//kliens j�t�kmez�i
	std::vector<PlayTile> enemyTiles; //ellenf�l j�t�kmez�i
	const int enemyTilesIndexOffset = 100; //offsetelni az ellenf�lmez� indexeket
	float textureAnimationOffset = 0.0f; //jelenleg mennyire mozgatjuk el a text�r�t

	const float mountainTileOffset = (2.0f * PlayTile::getScaleXZ()) * TorpedoGLOBAL::Scale; //hegy �s els� j�t�kmez� k�zti t�v
	const float mountaincenterBorderXOffset = Mountain::getWidthX() / 2.0f * TorpedoGLOBAL::Scale; //hegy k�zepe �s sz�le k�zti t�v
	//glm::vec3 seaTileScale = glm::vec3(tileScaleXZ, 1.0f, tileScaleXZ) * TorpedoGLOBAL::Scale; //nagy�t�s

	//static const float tileScaleXZ; //tengermez� sk�l�z�sa XZ tengelyek ment�n
	const glm::mat4 matWorld; //SeaTile vil�g transzform�ci�
	const glm::mat4 matWorldIT; //VT inverze
};
