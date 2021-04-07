#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include "../Globals.h"
#include "../Frontend/PlayTile.h"

class Ship
{
public:
	Ship(void);
	Ship(bool isBattleShip,bool ally, glm::vec3 battleShipTranslate);
	Ship(std::vector<PlayTile*> tiles,bool ally);
	~Ship(void);

	void Init(bool isAlly);
	void Draw(gCamera &camera, gShaderProgram &sh_program);

	std::vector<PlayTile*> getPlayTiles();
private:
	gVertexBuffer	vb_ship;

	bool isAlly;
	std::vector<PlayTile*> playTiles;

	glm::vec3 ship_translate = glm::vec3(0, 0, 0)* TorpedoGLOBAL::Scale;
	float ship_rotate = 0.0f;
	glm::vec3 ship_rotate_angle = glm::vec3(0, 1.0f, 0);
	glm::vec3 ship_scale = glm::vec3(3.0f, 1.25f, 1.5f) * TorpedoGLOBAL::Scale;

};
