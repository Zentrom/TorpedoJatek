#pragma once

#include "MenuRenderer.h"

#include "../../../Utility/gVertexBuffer.h"
#include <GL/glew.h>

#include <vector>
//#include <map>
#include <string>

//Menü egy állapotát nyilvántartó osztály
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
	//gVertexBuffer vb_decorator; //Nem klikkelhetõk modell adatai
	gVertexBuffer vb_clickable; //Klikkelhetõ modell adatai
	gShaderProgram sh_menu; //Menü shadere
	//std::vector<GLuint> decoratorTextures; //Nem klikkelhetõ objektumok textúrája
	std::vector<std::pair<std::string, GLuint>> clickableTextures; //Klikkelhetõ objektumok szövege és textúrája

	const int preProcessOffset = 100;

	float nextElementY = 0.4f;

	//int id; //Állapot enumból az azonosító
};