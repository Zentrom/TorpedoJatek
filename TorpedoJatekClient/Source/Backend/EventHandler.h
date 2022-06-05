#pragma once

#include "../Frontend/Fleet.h"

//J�t�kmenet esem�nykezel�je
class EventHandler
{
public:
	EventHandler();
	~EventHandler();

	void Update(float deltatime);
	void FireProjectile(Fleet &fleet, PlayTile *shottile);

	bool IsProjectileAnimation();
private:
	BShipProjectile* animatedProjectile;
	bool isProjectileAnimation = false; //L�ved�k anim�ci� folyamatban van-e
};