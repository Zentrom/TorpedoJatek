#pragma once

#include "MenuStateHandler.h"

#include "../../../Utility/GLUtils.h"
#include <SDL_mixer.h>

//Beállításokat kezelõ osztály
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

	gVertexBuffer vb_clickables; //Beállítások clickelhetõ elemei
	gShaderProgram sh_options; //Beállítások shadere
	GLuint resolutionTexture; //Felbontás szövege
	GLuint checkBoxOffTexture = GLUtils::TextureFromFile("Resources/Textures/Menu/checkBoxOff.bmp"); //Kikapcsolt checkbox textúrája
	GLuint checkBoxOnTexture = GLUtils::TextureFromFile("Resources/Textures/Menu/checkBoxOn.bmp"); //Bekapcsolt checkbox textúrája
	std::vector<std::pair<int, int>> displayModes; //Felbontásokat nyilvántartó gyûjtõ

	float musSliderTransX = 0.0f; //Zenehangerõ csúszkájának az elmozatási értéke
	float sfxSliderTransX = 0.0f; //Hangok hangerejének csúszkájának az elmozgatási értéke

	std::vector<std::pair<int, int>>::iterator displayModeIndex; //Éppen melyik felbontás páros van kiválasztva
	bool fullscreen; //Teljes képernyõ kivan-e jelölve
	bool vsync; //Vsync beállítás kivan-e jelölve
	float music; //Zene hangerõ értéke
	float sound; //Hangok hangereje

	std::map<std::string, int>& optionsRef; //Eltárolt beállítások mapje
};