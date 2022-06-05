#include "EventHandler.h"

EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
}

//Esem�nykezel� real-time friss�t�se
void EventHandler::Update(float deltatime)
{
	if (isProjectileAnimation) {
		if (!animatedProjectile->Animate(deltatime)) {
			isProjectileAnimation = false;
		}
	}
}

//L�ved�k anim�ci� ind�t�sa
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
