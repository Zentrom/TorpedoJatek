#pragma once

#include "PlayTile.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

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
	void Draw(gCamera &camera, gShaderProgram &sh_program);

	std::vector<PlayTile>& getTiles(bool ally = true);

protected:

	glm::vec3 calcTranslate(int rowNr, int columnNr, bool ally = true) const;

	int mapSize; //játékPálya mérete
	int tileCount;	//játékMezõk száma
	std::vector<PlayTile> myTiles;	//kliens játékmezõi
	std::vector<PlayTile> enemyTiles; //ellenfél játékmezõi

	float firstTile_battleShipOffset; //elsõ játékmezõ és csatahajó közti táv
	const float mountain_tile_offset = (4.0f + 1.0f)*TorpedoGLOBAL::Scale; //hegy és elsõ játékmezõ közti táv
	float mountaincenter_border_Xoffset = 6.0f / 2.0f*TorpedoGLOBAL::Scale; //hegy közepe és széle közti táv

};
