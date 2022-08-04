#pragma once

#include "../../Globals.hpp"

#include "../../../Utility/gVertexBuffer.h"
#include "../../../Utility/gShaderProgram.h"
#include "../../../Utility/gCamera.h"
#include "../../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//A Föld terület osztálya
class Terrain
{
public:
	Terrain(int seatile_row_count, float mountain_height_y);
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

	const int terrainScale = 50; //A terrainscale az feldarabolja az ugyanolyan nagy négyzetet

	const float groundScaleToSeaTile; //Tengermezõhöz képest mekkorát skálázunk
	const float groundMountainY; //hegy közepe és alja közti táv
	const int terrainSizeXZ; //hány darab földdarab legyen
	const glm::mat4 matWorld; //világ transzformáció
	const glm::mat4 matWorldIT; //VT inverze
	const float groundScaleXZ; //földdarab skálázása XZ tengelyeken

};
