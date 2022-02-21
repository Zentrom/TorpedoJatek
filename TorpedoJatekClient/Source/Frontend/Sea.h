#pragma once

#include "PlayTile.h"
#include "Mountain.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Tenger oszt�lya-2j�t�kos j�t�kmez�i+t�bbi tengermez�
class Sea
{
public:
	Sea(void);
	~Sea(void);

	void Init(int inMapSize = 7);
	void Draw(gCamera &camera, gShaderProgram &sh_program, float pointedTile);

	std::vector<PlayTile>& getTiles(bool ally = true);
	PlayTile& getTileByIndex(int index);

protected:
	void InitSeaTiles();
	void InitPlayTiles(int inMapSize);
	void RemoveExtraSeaTiles();
	bool CompareTileTranslations(SeaTile &sTile, PlayTile &pTile);
	glm::vec3 calcTranslate(int rowNr, int columnNr, bool ally = true) const;

	const int seaTileScale = 50 * 2; //h�ny tengermez� van egy sorba
	const int seaTileCount = seaTileScale * seaTileScale; //h�ny tengermez� van �sszesen
	std::vector<SeaTile> seaTiles; //Tengermez�ket tartalmazza

	int mapSize; //j�t�kP�lya m�rete
	int playTileCount;	//j�t�kMez�k sz�ma
	std::vector<PlayTile> myTiles;	//kliens j�t�kmez�i
	std::vector<PlayTile> enemyTiles; //ellenf�l j�t�kmez�i
	int enemyTilesIndexOffset = 100; //offsetelni az ellenf�lmez� indexeket

	const float mountain_tile_offset = (2 * SeaTile::getScaleXZ())*TorpedoGLOBAL::Scale; //hegy �s els� j�t�kmez� k�zti t�v
	const float mountaincenter_border_Xoffset = Mountain::getWidth() / 2.0f*TorpedoGLOBAL::Scale; //hegy k�zepe �s sz�le k�zti t�v

};
