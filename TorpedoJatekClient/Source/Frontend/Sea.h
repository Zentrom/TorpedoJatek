#pragma once

#include "PlayTile.h"

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
	void Draw(gCamera &camera, gShaderProgram &sh_program);

	std::vector<PlayTile>& getTiles(bool ally = true);

protected:

	glm::vec3 calcTranslate(int rowNr, int columnNr, bool ally = true) const;

	int mapSize; //j�t�kP�lya m�rete
	int tileCount;	//j�t�kMez�k sz�ma
	std::vector<PlayTile> myTiles;	//kliens j�t�kmez�i
	std::vector<PlayTile> enemyTiles; //ellenf�l j�t�kmez�i

	float firstTile_battleShipOffset; //els� j�t�kmez� �s csatahaj� k�zti t�v
	const float mountain_tile_offset = (4.0f + 1.0f)*TorpedoGLOBAL::Scale; //hegy �s els� j�t�kmez� k�zti t�v
	float mountaincenter_border_Xoffset = 6.0f / 2.0f*TorpedoGLOBAL::Scale; //hegy k�zepe �s sz�le k�zti t�v

};
