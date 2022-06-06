#pragma once

#include "ShipFlag.h"

#include "../Globals.hpp"
#include "../Frontend/PlayTile.h"

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
	Ship(void);
	Ship(const std::vector<PlayTile*> &tiles, bool ally = true);
	~Ship(void);

	void Update(float deltatime);
	void Draw(gCamera &camera, gShaderProgram &sh_program);

	std::vector<PlayTile*>& getPlayTiles();
	bool isDestroyed();
	bool isVisible();
	void setDestroyed(bool dis);
protected:
	void Init();

	gVertexBuffer	vb_ship; //Haj� grafikai modell adatai
	GLuint shipBottomTextureID; //Haj� alj�nak text�ra azonos�t�ja
	GLuint shipTopTextureID; //Haj� tetej�nek text�ra azonos�t�ja
	ShipFlag* shipFlag;

	bool isAlly; //Kliens szempontj�b�l mi haj�nk-e
	bool destroyed = false; //Ki lett-e l�ve a haj�
	std::vector<PlayTile*> playTiles;	//Mely j�t�kmez�k�n van a haj�

	bool visible = true; //Ki kell-e m�g rajzolni
	float sinkElapsed = 0.0f; //Eltelt id�
	float sinkTime = 30.0f; //Els�llyed�s ideje
	glm::mat4 sinkTranslate; //S�llyed�si mozg�s
	glm::mat4 sinkRotate; //S�llyed�si forg�s

	glm::vec3 ship_abovesea_trans = glm::vec3(0, 0.12f,0); //korrig�l� transz,hogy a haj� lebegjen a vizen.
	glm::vec3 ship_translate = glm::vec3(0, 0, 0)* TorpedoGLOBAL::Scale; //mozgat�s
	float ship_rotate = 0.0f; //forgat�s
	glm::vec3 ship_rotate_angle = glm::vec3(0, 1.0f, 0); //forgat�s tengelye
	glm::vec3 ship_scale = glm::vec3(1.0f); //nagy�t�s

};
