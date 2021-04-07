#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include "PlayTile.h"

class Sea
{
public:
	Sea(void);
	~Sea(void);

	void Init(int inMapSize);
	void Draw(gCamera &camera, gShaderProgram &sh_program);

	std::vector<PlayTile>& getTiles(bool ally);

protected:

	glm::vec3 calcTranslate(int rowNr,int columnNr,bool ally);

	int mapSize;
	int tileCount;
	std::vector<PlayTile> myTiles;
	std::vector<PlayTile> enemyTiles;

	float firstTile_battleShipOffset; //= 2.0f*mapSize*TorpedoGLOBAL::Scale;
	const float mountain_tile_offset = (4.0f + 1.0f)*TorpedoGLOBAL::Scale; //tilescale fele
	const float mountaincenter_border_Xoffset = 6.0f / 2.0f*TorpedoGLOBAL::Scale; //scale2irányba megy

};
