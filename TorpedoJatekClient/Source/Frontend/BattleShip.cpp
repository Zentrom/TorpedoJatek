
#include "BattleShip.h"

BattleShip::BattleShip(void)
{
}

BattleShip::BattleShip(bool ally, const glm::vec3 &battleShipTranslate)
{
	Init();
	shipFlag = new ShipFlag(ally);
	ship_translate = glm::vec3((ally ? -1 : 1), 1, 1) * battleShipTranslate;
	ship_scale = glm::vec3(1.5f * SeaTile::getScaleXZ(), 2.0f, 0.75f * SeaTile::getScaleXZ())
		* TorpedoGLOBAL::Scale; //nagy�t�s(ez most a battleship scale-je)
	cannon = new BShipCannon(ship_translate, ally, ship_scale.x);
}

BattleShip::~BattleShip(void)
{
}

//Kirajzol egy csatahaj�t
void BattleShip::Draw(gCamera &camera, gShaderProgram &sh_program)
{
	Ship::Draw(camera, sh_program);
	cannon->Draw(camera, sh_program);
}

BShipCannon& BattleShip::getCannon()
{
	return *cannon;
}
