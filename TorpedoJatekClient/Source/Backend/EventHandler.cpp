#include "EventHandler.h"

EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
}

//Eseménykezelõ real-time frissítése
void EventHandler::Update(float deltatime)
{
	if (isProjectileAnimation) {
		if (!animatedProjectile->Animate(deltatime)) {
			isProjectileAnimation = false;
		}
	}
}

//Lövedék animáció indítása
void EventHandler::FireProjectile(Fleet &fleet, PlayTile *shotTile)
{
	animatedProjectile = &fleet.getBattleShip().getCannon().getProjectile();
	animatedProjectile->Fire(shotTile->getTranslate());
	isProjectileAnimation = true;
}

bool EventHandler::IsProjectileAnimation()
{
	return isProjectileAnimation;
}
