#pragma once

#include "../Frontend/Fleet.h"

#include <SDL_mixer.h>

//Játékmenet eseménykezelõje
class EventHandler
{
public:
	EventHandler();
	~EventHandler();

	void Update(float deltatime, glm::vec3 cameye);
	void FireProjectile(Fleet*& fleet, PlayTile *shottile);

	bool IsProjectileAnimation();
private:
	BShipProjectile* animatedProjectile; //Az éppen animált lövedék
	bool isProjectileAnimation = false; //Lövedék animáció folyamatban van-e

	//Mix_Music* music; //Zeneadatra mutató pointer
	Mix_Chunk* hitSound; //Találat hangadatra mutató pointer
	Mix_Chunk* missSound; //Mellé-lövés hangadatra mutató pointer

	//Mix_Chunk** nextSound; //Következõ lejátszandó hang
	glm::vec3 tilePos; //Mezõ helye a hang lejátszásához
};