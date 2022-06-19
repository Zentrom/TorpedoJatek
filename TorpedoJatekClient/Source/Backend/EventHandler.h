#pragma once

#include "../Frontend/Fleet.h"

#include <SDL_mixer.h>

//J�t�kmenet esem�nykezel�je
class EventHandler
{
public:
	EventHandler();
	~EventHandler();

	void Update(float deltatime, glm::vec3 cameye);
	void FireProjectile(Fleet*& fleet, PlayTile *shottile);

	bool IsProjectileAnimation();
private:
	BShipProjectile* animatedProjectile; //Az �ppen anim�lt l�ved�k
	bool isProjectileAnimation = false; //L�ved�k anim�ci� folyamatban van-e

	//Mix_Music* music; //Zeneadatra mutat� pointer
	Mix_Chunk* hitSound; //Tal�lat hangadatra mutat� pointer
	Mix_Chunk* missSound; //Mell�-l�v�s hangadatra mutat� pointer

	//Mix_Chunk** nextSound; //K�vetkez� lej�tszand� hang
	glm::vec3 tilePos; //Mez� helye a hang lej�tsz�s�hoz
};