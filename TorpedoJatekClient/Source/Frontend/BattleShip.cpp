
#include "BattleShip.h"

BattleShip::BattleShip(const glm::vec3 &battle_ship_translate, bool ally) : Ship::Ship(ally)
{
	shipTranslate = glm::vec3((ally ? -1 : 1), 1, 1) * battle_ship_translate;
	shipScale = glm::vec3(1.5f * TorpedoGLOBAL::SeaTileScaleXZ, 2.0f, 0.75f * TorpedoGLOBAL::SeaTileScaleXZ)
		* TorpedoGLOBAL::Scale; //nagyítás(ez most a battleship scale-je)
	cannon = new BShipCannon(shipTranslate, ally, shipScale.x);

	Init();
}

BattleShip::~BattleShip()
{
	delete cannon;
}

//Kirajzol egy csatahajót
void BattleShip::Draw(const gCamera& camera, gShaderProgram& sh_program)
{
	Ship::Draw(camera, sh_program);
	glm::mat4 cann_rotate = glm::rotate((sinkElapsed / sinkTime) * glm::half_pi<float>() / 2.0f,
		glm::vec3(0, 0, 1.0f));
	cannon->Draw(camera, sh_program, sinkTranslate * cann_rotate);
}

//DEBUG módhoz visszaállítja a hajót
void BattleShip::ResetForDEBUG()
{
	visible = true;
	sinkElapsed = 0.0f;
}

const BShipCannon& BattleShip::getCannon() const
{
	return *cannon;
}
