#pragma once

#include "../Globals.h"
#include "../Frontend/PlayTile.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Haj� oszt�lya
class Ship
{
public:
	Ship(void);
	Ship(bool isBattleShip, bool ally, const glm::vec3 &battleShipTranslate);
	Ship(const std::vector<PlayTile*> &tiles, bool ally = true);
	~Ship(void);

	void Init(bool isAlly = true);
	void Draw(gCamera &camera, gShaderProgram &sh_program);

	std::vector<PlayTile*>& getPlayTiles();
private:
	gVertexBuffer	vb_ship; //Haj� grafikai modell adatai

	bool isAlly; //Kliens szempontj�b�l mi haj�nk-e
	std::vector<PlayTile*> playTiles;	//Mely j�t�kmez�k�n van a haj�

	glm::vec3 ship_translate = glm::vec3(0, 0, 0)* TorpedoGLOBAL::Scale; //mozgat�s
	float ship_rotate = 0.0f; //forgat�s
	glm::vec3 ship_rotate_angle = glm::vec3(0, 1.0f, 0); //forgat�s tengelye
	glm::vec3 ship_scale = glm::vec3(3.0f, 1.25f, 1.5f) * TorpedoGLOBAL::Scale; //nagy�t�s

};
