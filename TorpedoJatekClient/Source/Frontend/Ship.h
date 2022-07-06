#pragma once

#include "../Globals.hpp"
#include "PlayTile.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"
#include "../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Haj� oszt�lya
class Ship
{
public:
	Ship(bool ally);
	Ship(const std::vector<PlayTile*> &tiles, bool ally = true);
	~Ship();

	void Update(float delta_time);
	void Draw(const gCamera& camera, gShaderProgram& sh_program, const gVertexBuffer& vb_ship, const gVertexBuffer& vb_flag) const;

	std::vector<PlayTile*>& getPlayTiles();
	bool isDestroyed() const;
	bool isVisible() const;
	void setDestroyed(bool dis);
	glm::vec3 getShipTranslate();
protected:
	void Init();

	GLuint shipBottomTextureID; //Haj� alj�nak text�ra azonos�t�ja
	GLuint shipTopTextureID; //Haj� tetej�nek text�ra azonos�t�ja

	const float sinkTime = 30.0f; //Els�llyed�s ideje
	const glm::vec3 shipAboveSeaTrans = glm::vec3(0, 0.12f, 0); //korrig�l� transz,hogy a haj� lebegjen a vizen.

	glm::mat4 sinkTranslate = glm::mat4(1.0f); //S�llyed�si mozg�s
	glm::mat4 sinkRotate = glm::mat4(1.0f); //S�llyed�si forg�s
	glm::vec3 shipTranslate = glm::vec3(0, 0, 0)* TorpedoGLOBAL::Scale; //mozgat�s
	float shipRotate = 0.0f; //forgat�s
	glm::vec3 shipRotateAngle = glm::vec3(0, 1.0f, 0); //forgat�s tengelye
	glm::vec3 shipScale = glm::vec3(1.0f); //nagy�t�s
	glm::mat4 matWorld; //Vil�g transzform�ci�
	glm::mat4 matWorldIT; //VT inverze

	bool destroyed = false; //Ki lett-e l�ve a haj�
	bool visible = true; //Ki kell-e m�g rajzolni
	float sinkElapsed = 0.0f; //Eltelt id�

	std::vector<PlayTile*> playTiles;	//Mely j�t�kmez�k�n van a haj�
	bool isAlly; //Kliens szempontj�b�l mi haj�nk-e

};
