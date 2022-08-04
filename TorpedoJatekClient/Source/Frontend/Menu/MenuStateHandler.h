#pragma once

#include "MenuRenderer.h"

#include "../../../Utility/gVertexBuffer.h"
#include <GL/glew.h>

#include <vector>
//#include <map>
#include <string>

//Men� egy �llapot�t nyilv�ntart� oszt�ly
class MenuStateHandler 
{
public:
	MenuStateHandler();
	~MenuStateHandler();

	void PreProcess();
	void Render();

	void AddButton(const char* name);
	void AddInputBox(const char* name);
	void BuildLayout();
	void UpdateTexture(int id, const char* next_char, bool is_delete);

	std::vector<std::string> getInputStrings(int count);
private:

	MenuRenderer* menuRenderer = new MenuRenderer();
	//gVertexBuffer vb_decorator; //Nem klikkelhet�k modell adatai
	gVertexBuffer vb_clickable; //Klikkelhet� modell adatai
	gShaderProgram sh_menu; //Men� shadere
	//std::vector<GLuint> decoratorTextures; //Nem klikkelhet� objektumok text�r�ja
	std::vector<std::pair<std::string, GLuint>> clickableTextures; //Klikkelhet� objektumok sz�vege �s text�r�ja

	const int preProcessOffset = 100;

	float nextElementY = 0.4f;

	//int id; //�llapot enumb�l az azonos�t�
};