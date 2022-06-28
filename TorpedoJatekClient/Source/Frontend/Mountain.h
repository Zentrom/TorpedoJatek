#pragma once

#include "../Globals.hpp"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"
#include "../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//A hegy oszt�lya
class Mountain
{
public:
	Mountain();
	~Mountain();

	void Init();
	void Draw(const gCamera& camera, gShaderProgram& sh_program) const;

	static float getHeight();
	static float getWidthX();
	const float getWidthZ() const;

private:
	gVertexBuffer vb_mountain; //grafikai modell adatok
	GLuint mountainTextureID; //Hegy text�ra azonos�t�ja

	const int mountainResolution = 32; //h�ny darab n�gyzetb�l �lljon a hegy egy sora/oszlopa
	const float widthZ = 20.0f; //Sz�less�g Z tengelyen

	static const float heightY; //magass�g
	static const float widthX; //Hegy sz�less�ge X tengelyen
	const glm::vec3 mountainScale; //nagy�t�s
	const glm::mat4 matWorld; //vil�g transzform�ci�
	const glm::mat4 matWorldIT; //VT inverze

};