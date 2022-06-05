#pragma once

#include "../Frontend/Fleet.h"

//Játékmenet eseménykezelõje
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
	bool isProjectileAnimation = false; //Lövedék animáció folyamatban van-e
};