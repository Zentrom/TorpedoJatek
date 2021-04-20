#pragma once

#include "Ground.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//A f�ld oszt�lya(darabokat �sszegy�jti)
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
	//A terrainscale az feldarabolja az ugyanolyan nagy n�gyzetet
	const int terrainScale = 3;
	const int terrainMultiplier = terrainScale*terrainScale; //n�gyzetsz�m legyen
	const int terrainSize = mapSize*mapSize*terrainMultiplier; //h�ny darab f�lddarab legyen
	std::vector<Ground> myGrounds; //f�lddarabok

};
