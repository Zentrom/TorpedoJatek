#pragma once

#include "MenuStateHandler.h"

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
private:

	gVertexBuffer vb_clickables; //Beállítások clickelhetõ elemei
	gShaderProgram sh_options; //Beállítások shadere
	GLuint resolutionTexture; //Felbontás szövege
	std::vector<std::pair<int, int>> displayModes;
	std::vector<std::pair<int, int>>::iterator displayModeIndex;

	std::map<std::string, int>& optionsRef;
};