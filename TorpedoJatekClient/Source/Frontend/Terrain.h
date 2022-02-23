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

//A f�ld oszt�lya(darabokat �sszegy�jti)
class Terrain
{
public:
	Terrain(void);
	~Terrain(void);

	void Init();
	void Draw(gCamera &camera, gShaderProgram &sh_program);

	int getTerrainScale();

protected:
	GLuint groundTextureID; //f�lddarab text�ra azonos�t�ja
	const float ground_mountainY = (Mountain::getHeight() / -2.0f - 0.01f) * TorpedoGLOBAL::Scale; //hegy k�zepe �s alja k�zti t�v
	//A terrainscale az feldarabolja az ugyanolyan nagy n�gyzetet
	const int terrainScale = 50;
	const int terrainSizeXZ = terrainScale*terrainScale; //h�ny darab f�lddarab legyen
	std::vector<Ground> myGrounds; //f�lddarabok

};
