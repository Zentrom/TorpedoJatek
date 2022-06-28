
#include "BattleShip.h"

//BattleShip::BattleShip(void)
//{
//}

BattleShip::BattleShip(const glm::vec3 &battle_ship_translate, bool ally) : Ship::Ship(ally)
{
	//shipFlag = new ShipFlag(ally);
	shipTranslate = glm::vec3((ally ? -1 : 1), 1, 1) * battle_ship_translate;
	shipScale = glm::vec3(1.5f * PlayTile::getScaleXZ(), 2.0f, 0.75f * PlayTile::getScaleXZ())
		* TorpedoGLOBAL::Scale; //nagy�t�s(ez most a battleship scale-je)
	cannon = new BShipCannon(shipTranslate, ally, shipScale.x);

	Init();
}

BattleShip::~BattleShip()
{
	delete cannon;
}

//Kirajzol egy csatahaj�t
void BattleShip::Draw(const gCamera& camera, gShaderProgram& sh_program)
{
	Ship::Draw(camera, sh_program);
	cannon->Draw(camera, sh_program, sinkTranslate * sinkRotate);
}

//DEBUG m�dhoz vissza�ll�tja a haj�t
void BattleShip::ResetForDEBUG()
{
	visible = true;
	sinkElapsed = 0.0f;
}

const BShipCannon& BattleShip::getCannon() const
{
	return *cannon;
}
