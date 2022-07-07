#pragma once

#include "../Frontend/Fleet.h"

#include <SDL_mixer.h>

//Játékmenet eseménykezelõje
class EventHandler
{
public:
	EventHandler();
	~EventHandler();

	void Update(float delta_time, const glm::vec3& cam_eye);
	void FireProjectile(Fleet& fleet, PlayTile& shot_tile);
	void SwitchVolume();

	bool IsProjectileAnimation() const;
private:
	void ApplySoundDistEffect(int channel_num, float sound_pos_x, float sound_pos_z);

	BShipProjectile* pAnimatedProjectile = nullptr; //Az éppen animált lövedék
	BShipProjectile* pLastAnimatedProjectile = nullptr; //Elõzõleg animált lövedék
	PlayTile* pShotTile = nullptr; //Éppen lött mezõ
	Mix_Music* music; //Zeneadatra mutató pointer
	//1-es Mixer channelen: (8 channel allokált alapból)
	Mix_Chunk* hitSound; //Találat hangadatra mutató pointer
	Mix_Chunk* missSound; //Mellé-lövés hangadatra mutató pointer
	//2-es channel:
	Mix_Chunk* cannonFireSound; //Ágyúlövés hangadatra mutató pointer

	bool isMuted = false; //Némítás

	//A hangforrások hangját szabályozza(végén channel hangerõvel mixelõdik)
	const int musicVolume = MIX_MAX_VOLUME / 4;
	const int hitVolume = MIX_MAX_VOLUME;
	const int missVolume = MIX_MAX_VOLUME / 2;
	const int cannonFireVolume = MIX_MAX_VOLUME / 2;
	float soundScanDistance = 40.0f; //Mekkora távolságon belül interpoláljuk a hangerõt
	const int musicFadeInTimeMs = 5000; //Zene fade-in effectje millisecben

	bool isProjectileAnimation = false; //Lövedék animáció folyamatban van-e
	bool isPostParticleAnimation = false; //Lövés utáni részecskék animálása van-e
	float camEyeX; //Kamera pos X tengelyen
	float camEyeZ; //Kamera pos Z tengelyen
	
};