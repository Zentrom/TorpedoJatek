#pragma once

#include "Mountain.h"
#include "SeaTile.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"
#include "../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//A Föld terület osztálya
class Terrain
{
public:
	Terrain();
	~Terrain();

	void Init();
	void Draw(const gCamera& camera, gShaderProgram& sh_program) const;

	const int getTerrainScale() const;
	const float getGroundScaleXZ() const;

private:
	void CalcGroundTileTransformations();

	gVertexBuffer vb_terrain; //grafikai adatokat tároló buffer
	GLuint groundTextureID; //földdarab textúra azonosítója
	std::vector<glm::vec3> groundTileTrans; //földdarabok elmozgazásai

	const glm::mat4 matWorld; //világ transzformáció
	const glm::mat4 matWorldIT; //VT inverze

	const int terrainScale = 50; //A terrainscale az feldarabolja az ugyanolyan nagy négyzetet
	const int terrainSizeXZ; //hány darab földdarab legyen

	const float groundMountainY = (Mountain::getHeight() / -2.0f - 0.01f) * TorpedoGLOBAL::Scale; //hegy közepe és alja közti táv
	const float groundScaleToSeaTile = 2.0f; //Tengermezõhöz képest mekkorát skálázunk
	const float groundScaleXZ; //földdarab skálázása XZ tengelyeken
	
};
