#pragma once

//#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../Utils/gVertexBuffer.h"
#include "../../Utils/gShaderProgram.h"
#include "../../Utils/gCamera.h"

#include "Ground.h"

class Terrain
{
public:
	Terrain(void);
	~Terrain(void);

	void Init();
	void Draw(gCamera &camera, gShaderProgram &sh_program);

protected:

	//const int mapSize = GLOBALMapSize;
	const int terrainScale = 3;

	const int terrainMultiplier = terrainScale*terrainScale; //négyzetszám legyen
	const int terrainSize = TorpedoGLOBAL::MapSize*TorpedoGLOBAL::MapSize*terrainMultiplier;
	Ground* myGrounds;
	//std::array<Ground, 7> myGrounds = new Ground();


	//gVertexBuffer	vb_mountain;
	//glm::vec3 mountain_translate = glm::vec3(0, 0, 0);
	//float mountain_rotate = 0.0f;
	//glm::vec3 mountain_rotate_angle = glm::vec3(0, 1.0f, 0);
	//glm::vec3 mountain_scale = glm::vec3(6.0f, 6.0f, 20.0f);

};
