#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include "Ground.h"

class Terrain
{
public:
	Terrain(void);
	~Terrain(void);

	void Init();
	void Draw(gCamera &camera, gShaderProgram &sh_program);

protected:

	//Ideiglenes.A mapsize nem fogja befolyasolni a fold meretet.
	const int mapSize = 7;
	//A terrainscale az feldarabolja az ugyanolyan nagy négyzetet
	const int terrainScale = 3;
	const int terrainMultiplier = terrainScale*terrainScale; //négyzetszám legyen
	const int terrainSize = mapSize*mapSize*terrainMultiplier;
	Ground* myGrounds;
	//std::array<Ground, terrainSize> *myGrounds = new std::array<Ground, 7>();

};
