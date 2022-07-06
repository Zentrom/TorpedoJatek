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

//Hajó osztálya
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

	GLuint shipBottomTextureID; //Hajó aljának textúra azonosítója
	GLuint shipTopTextureID; //Hajó tetejének textúra azonosítója

	const float sinkTime = 30.0f; //Elsüllyedés ideje
	const glm::vec3 shipAboveSeaTrans = glm::vec3(0, 0.12f, 0); //korrigáló transz,hogy a hajó lebegjen a vizen.

	glm::mat4 sinkTranslate = glm::mat4(1.0f); //Süllyedési mozgás
	glm::mat4 sinkRotate = glm::mat4(1.0f); //Süllyedési forgás
	glm::vec3 shipTranslate = glm::vec3(0, 0, 0)* TorpedoGLOBAL::Scale; //mozgatás
	float shipRotate = 0.0f; //forgatás
	glm::vec3 shipRotateAngle = glm::vec3(0, 1.0f, 0); //forgatás tengelye
	glm::vec3 shipScale = glm::vec3(1.0f); //nagyítás
	glm::mat4 matWorld; //Világ transzformáció
	glm::mat4 matWorldIT; //VT inverze

	bool destroyed = false; //Ki lett-e lõve a hajó
	bool visible = true; //Ki kell-e még rajzolni
	float sinkElapsed = 0.0f; //Eltelt idõ

	std::vector<PlayTile*> playTiles;	//Mely játékmezõkön van a hajó
	bool isAlly; //Kliens szempontjából mi hajónk-e

};
