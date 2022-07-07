#pragma once

#include "../Frontend/Fleet.h"

#include <SDL_mixer.h>

//J�t�kmenet esem�nykezel�je
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

	BShipProjectile* pAnimatedProjectile = nullptr; //Az �ppen anim�lt l�ved�k
	BShipProjectile* pLastAnimatedProjectile = nullptr; //El�z�leg anim�lt l�ved�k
	PlayTile* pShotTile = nullptr; //�ppen l�tt mez�
	Mix_Music* music; //Zeneadatra mutat� pointer
	//1-es Mixer channelen: (8 channel allok�lt alapb�l)
	Mix_Chunk* hitSound; //Tal�lat hangadatra mutat� pointer
	Mix_Chunk* missSound; //Mell�-l�v�s hangadatra mutat� pointer
	//2-es channel:
	Mix_Chunk* cannonFireSound; //�gy�l�v�s hangadatra mutat� pointer

	bool isMuted = false; //N�m�t�s

	//A hangforr�sok hangj�t szab�lyozza(v�g�n channel hanger�vel mixel�dik)
	const int musicVolume = MIX_MAX_VOLUME / 4;
	const int hitVolume = MIX_MAX_VOLUME;
	const int missVolume = MIX_MAX_VOLUME / 2;
	const int cannonFireVolume = MIX_MAX_VOLUME / 2;
	float soundScanDistance = 40.0f; //Mekkora t�vols�gon bel�l interpol�ljuk a hanger�t
	const int musicFadeInTimeMs = 5000; //Zene fade-in effectje millisecben

	bool isProjectileAnimation = false; //L�ved�k anim�ci� folyamatban van-e
	bool isPostParticleAnimation = false; //L�v�s ut�ni r�szecsk�k anim�l�sa van-e
	float camEyeX; //Kamera pos X tengelyen
	float camEyeZ; //Kamera pos Z tengelyen
	
};