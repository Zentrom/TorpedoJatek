#pragma once

#include "Ship.h"
#include "BShipCannon.h"

//Egy lövõ hajó
class BattleShip : public Ship
{
public:
	BattleShip(void);
	BattleShip(bool ally, const glm::vec3 &battleShipTranslate);
	~BattleShip(void);

	void Draw(gCamera& camera, gShaderProgram& sh_program);
	void ResetForDEBUG();
	BShipCannon& getCannon();
private:
	BShipCannon* cannon;
};