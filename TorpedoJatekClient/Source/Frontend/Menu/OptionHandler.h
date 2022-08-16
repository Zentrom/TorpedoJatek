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
	GLuint checkBoxOffTexture = GLUtils::TextureFromFile("Resources/Textures/Menu/checkBoxOff.bmp"); //Kikapcsolt checkbox text�r�ja
	GLuint checkBoxOnTexture = GLUtils::TextureFromFile("Resources/Textures/Menu/checkBoxOn.bmp"); //Bekapcsolt checkbox text�r�ja
	std::vector<std::pair<int, int>> displayModes; //Felbont�sokat nyilv�ntart� gy�jt�

	float musSliderTransX = 0.0f; //Zenehanger� cs�szk�j�nak az elmozat�si �rt�ke
	float sfxSliderTransX = 0.0f; //Hangok hangerej�nek cs�szk�j�nak az elmozgat�si �rt�ke

	std::vector<std::pair<int, int>>::iterator displayModeIndex; //�ppen melyik felbont�s p�ros van kiv�lasztva
	bool fullscreen; //Teljes k�perny� kivan-e jel�lve
	bool vsync; //Vsync be�ll�t�s kivan-e jel�lve
	float music; //Zene hanger� �rt�ke
	float sound; //Hangok hangereje

	std::map<std::string, int>& optionsRef; //Elt�rolt be�ll�t�sok mapje
};