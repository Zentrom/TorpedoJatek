#pragma once

#include "MenuStateHandler.h"

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
private:

	gVertexBuffer vb_clickables; //Be�ll�t�sok clickelhet� elemei
	gShaderProgram sh_options; //Be�ll�t�sok shadere
	GLuint resolutionTexture; //Felbont�s sz�vege
	std::vector<std::pair<int, int>> displayModes;
	std::vector<std::pair<int, int>>::iterator displayModeIndex;

	std::map<std::string, int>& optionsRef;
};