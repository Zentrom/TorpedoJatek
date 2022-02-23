#pragma once

#include "Ground.h"
#include "Mountain.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"
#include "../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//A föld osztálya(darabokat összegyûjti)
class Terrain
{
public:
	Terrain(void);
	~Terrain(void);

	void Init();
	void Draw(gCamera &camera, gShaderProgram &sh_program);

	int getTerrainScale();

protected:
	GLuint groundTextureID; //földdarab textúra azonosítója
	const float ground_mountainY = (Mountain::getHeight() / -2.0f - 0.01f) * TorpedoGLOBAL::Scale; //hegy közepe és alja közti táv
	//A terrainscale az feldarabolja az ugyanolyan nagy négyzetet
	const int terrainScale = 50;
	const int terrainSizeXZ = terrainScale*terrainScale; //hány darab földdarab legyen
	std::vector<Ground> myGrounds; //földdarabok

};
