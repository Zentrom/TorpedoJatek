#pragma once

#include "../../Globals.hpp"

#include "../../../Utility/gVertexBuffer.h"
#include "../../../Utility/gShaderProgram.h"
#include "../../../Utility/gCamera.h"
#include "../../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//A hegy osztálya
class Mountain
{
public:
	Mountain();
	~Mountain();

	void Init();
	void Draw(const gCamera& camera, gShaderProgram& sh_program) const;

	const float getWidthZ() const;
	const float getHeightY() const;
private:
	gVertexBuffer vb_mountain; //grafikai modell adatok
	GLuint mountainTextureID; //Hegy textúra azonosítója

	const int mountainResolution = 32; //hány darab négyzetbõl álljon a hegy egy sora/oszlopa
	const float widthX = 6.0f; //Szélesség X tengelyen
	const float heightY = 8.0f; //Magasság Y tengelyen
	const float widthZ = 20.0f; //Szélesség Z tengelyen

	const glm::vec3 mountainScale; //nagyítás
	const glm::mat4 matWorld; //világ transzformáció
	const glm::mat4 matWorldIT; //VT inverze

};