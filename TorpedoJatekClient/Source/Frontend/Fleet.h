#pragma once

//#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../Utils/gVertexBuffer.h"
#include "../../Utils/gShaderProgram.h"
#include "../../Utils/gCamera.h"

#include "Ship.h"

class Fleet
{
public:
	Fleet(void);
	//Fleet(int* actPlayTiles);
	~Fleet(void);

	void Init(int* actPlayTiles);
	void Draw(gCamera &m_camera, gShaderProgram &m_program);

protected:
	Ship myShips[16];
	Ship enemyShips[16];

	Ship myBattleShip;
	Ship enemyBattleShip;

	float firstTile_battleShipOffset = 2.0f*7.0f;
	const float mountain_tile_offset = 4.0f + 1.0f; //tilescale fele
	const float mountaincenter_border_Xoffset = 6.0f / 2.0f; //scale2irányba megy


	//gVertexBuffer	vb_mountain;
	//glm::vec3 mountain_translate = glm::vec3(0, 0, 0);
	//float mountain_rotate = 0.0f;
	//glm::vec3 mountain_rotate_angle = glm::vec3(0, 1.0f, 0);
	//glm::vec3 mountain_scale = glm::vec3(6.0f, 6.0f, 20.0f);

};
