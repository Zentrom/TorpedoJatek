#pragma once

#include "MenuStateHandler.h"

#include "../../../Utility/GLUtils.h"
#include <SDL_mixer.h>

//Be�ll�t�sokat kezel� oszt�ly
class OptionHandler : public MenuStateHandler 
{
public:
	OptionHandler(std::map<std::string, int>& options);
	~OptionHandler();

	void PreProcess();
	void Render(float pointed_element);

	void AddClickableOptions();
	void SwitchResolution(bool forward);
	void SwitchFullscreen();
	void SwitchVsync();
	void HandleSlider(float pointed, Sint32 mouse_x);
	void UpdateVolume();
	void ApplySettings(SDL_Window* window);
	void CancelSettings();
private:

	gVertexBuffer vb_clickables; //Be�ll�t�sok clickelhet� elemei
	gShaderProgram sh_options; //Be�ll�t�sok shadere
	GLuint resolutionTexture; //Felbont�s sz�vege
	GLuint checkBoxOffTexture = GLUtils::TextureFromFile("Resources/Textures/Menu/checkBoxOff.bmp");
	GLuint checkBoxOnTexture = GLUtils::TextureFromFile("Resources/Textures/Menu/checkBoxOn.bmp");
	std::vector<std::pair<int, int>> displayModes;

	float musSliderTransX = 0.0f;
	float sfxSliderTransX = 0.0f;

	std::vector<std::pair<int, int>>::iterator displayModeIndex;
	bool fullscreen;
	bool vsync;
	float music;
	float sound;

	std::map<std::string, int>& optionsRef;
};