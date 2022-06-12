#include "EventHandler.h"

EventHandler::EventHandler()
{
	hitSound = Mix_LoadWAV("Resources/Audio/hitSound.wav");
	if (!hitSound) {
		printf("Mix_LoadWAV error: %s\n", SDL_GetError());
	}
	missSound = Mix_LoadWAV("Resources/Audio/missSound.wav");
	if (!missSound) {
		printf("Mix_LoadWAV error: %s\n", SDL_GetError());
	}
	if (!TorpedoGLOBAL::AudioEnabled) {
		Mix_VolumeChunk(hitSound, 0);
		Mix_VolumeChunk(missSound, 0);
	}
}

EventHandler::~EventHandler()
{
	Mix_FreeChunk(hitSound);
	Mix_FreeChunk(missSound);
}

//Eseménykezelõ real-time frissítése
void EventHandler::Update(float deltatime, glm::vec3 cameye)
{
	if (isProjectileAnimation) {
		if (!animatedProjectile->Animate(deltatime)) {
			isProjectileAnimation = false;

			float scannedDistance = 40.0f;
			float len = glm::length(glm::vec3(cameye.x - tilePos.x, 0
				, cameye.z - tilePos.z));
			int ds = (len > scannedDistance ? (scannedDistance - 1) : len) * 256 / scannedDistance;
			Mix_SetDistance(2, ds);
			//JAVÍTANI MAJD nextSoundra
			Mix_PlayChannel(2, missSound, 0);
		}
	}
}

//Lövedék animáció indítása
void EventHandler::FireProjectile(Fleet &fleet, PlayTile *shotTile)
{
	animatedProjectile = &fleet.getBattleShip().getCannon().getProjectile();
	animatedProjectile->Fire(shotTile->getTranslate());
	isProjectileAnimation = true;

	//Ha van benne zöld akkor missSound lesz
	//if (shotTile->getState().g == 1) {
	//	nextSound = missSound;
	//}
	//else {
	//	nextSound = hitSound;
	//}
	tilePos = shotTile->getTranslate();
}

bool EventHandler::IsProjectileAnimation()
{
	return isProjectileAnimation;
}
