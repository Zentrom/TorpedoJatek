#include "EventHandler.h"

EventHandler::EventHandler() : musicVolume(Mix_VolumeMusic(-1) / 2), sfxVolume(Mix_Volume(1, -1))
{
	music = Mix_LoadMUS("Resources/Audio/mainMusic.ogg");
	if (!music) {
		printf("Mix_LoadMUS(\"mainMusic.ogg\"): %s\n", Mix_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Mix_LoadMUS(\"mainMusic.ogg\")", SDL_GetError(), nullptr);
	}

	hitSound = Mix_LoadWAV("Resources/Audio/hitSound.wav");
	if (!hitSound) {
		printf("Mix_LoadWAV error: %s\n", SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Mix_LoadWAV error", SDL_GetError(), nullptr);
	}
	missSound = Mix_LoadWAV("Resources/Audio/missSound.wav");
	if (!missSound) {
		printf("Mix_LoadWAV error: %s\n", SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Mix_LoadWAV error", SDL_GetError(), nullptr);
	}
	cannonFireSound = Mix_LoadWAV("Resources/Audio/cannonFire.wav");
	if (!cannonFireSound) {
		printf("Mix_LoadWAV error: %s\n", SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Mix_LoadWAV error", SDL_GetError(), nullptr);
	}
	//Ez hogy az elején be lehessen állítani a headerbe
	isMuted = !isMuted;
	SwitchVolume();

	Mix_FadeInMusic(music, 3, musicFadeInTimeMs);

}

EventHandler::~EventHandler()
{
	Mix_FreeMusic(music);

	Mix_FreeChunk(hitSound);
	Mix_FreeChunk(missSound);
	Mix_FreeChunk(cannonFireSound);
}

//Eseménykezelõ real-time frissítése
void EventHandler::Update(float delta_time, const glm::vec3& cam_eye)
{
	camEyeX = cam_eye.x;
	camEyeZ = cam_eye.z;

	if (isProjectileAnimation) {
		if (!pAnimatedProjectile->Animate(delta_time)) {
			isProjectileAnimation = false;
			isPostParticleAnimation = true;

			ApplySoundDistEffect(1, pShotTile->getTranslate().x, pShotTile->getTranslate().z);
			if (pShotTile->getState() == 1 || pShotTile->getState() == 4 || pShotTile->getState() == 5) {
				Mix_PlayChannel(1, hitSound, 0);
			}
			else {
				Mix_PlayChannel(1, missSound, 0);
			}
		}
		if (pLastAnimatedProjectile && pLastAnimatedProjectile != pAnimatedProjectile) {
			pLastAnimatedProjectile->AnimPostParticles(delta_time);
		}
	}
	else if (isPostParticleAnimation) {
		bool firstIf = false;
		if (!pAnimatedProjectile->AnimPostParticles(delta_time)) {
			firstIf = true;
		}
		if (pLastAnimatedProjectile && pLastAnimatedProjectile != pAnimatedProjectile 
			&& !pLastAnimatedProjectile->AnimPostParticles(delta_time) && firstIf) {
			isPostParticleAnimation = false;
			pAnimatedProjectile->ClearParticles();
			pLastAnimatedProjectile->ClearParticles();
			pLastAnimatedProjectile = nullptr;
		}
	}
}

//Lövedék animáció indítása
void EventHandler::FireProjectile(Fleet& fleet, PlayTile& shot_tile)
{
	if (isPostParticleAnimation) {
		pLastAnimatedProjectile = pAnimatedProjectile;
	}
	pAnimatedProjectile = &fleet.getBattleShip().getCannon().getProjectile();
	pAnimatedProjectile->Fire(shot_tile.getTranslate());
	isProjectileAnimation = true;

	ApplySoundDistEffect(2, fleet.getBattleShip().getShipTranslate().x, fleet.getBattleShip().getShipTranslate().z);
	Mix_PlayChannel(2, cannonFireSound, 0);

	pShotTile = &shot_tile;
}

void EventHandler::ApplySoundDistEffect(int channel_num, float sound_pos_x, float sound_pos_z)
{
	float len = glm::length(glm::vec3(camEyeX - sound_pos_x, 0
		, camEyeZ - sound_pos_z));
	float dis = (len > soundScanDistance ? soundScanDistance : len) * 255 / soundScanDistance;
	Mix_SetDistance(channel_num, static_cast<int>(dis));
}

//Muteolja vagy reseteli az audio hangerõt - Zene és channelek
void EventHandler::SwitchVolume()
{
	if (!isMuted) {
		Mix_VolumeMusic(0);
		Mix_Volume(-1, 0);
		isMuted = true;
	}
	else {
		Mix_VolumeMusic(musicVolume);
		Mix_Volume(-1, sfxVolume);
		isMuted = false;
	}
}

bool EventHandler::IsProjectileAnimation() const
{
	return isProjectileAnimation;
}
