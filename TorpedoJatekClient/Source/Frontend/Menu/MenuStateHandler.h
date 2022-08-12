#pragma once

#include "MenuRenderer.h"

#include "../../../Utility/gVertexBuffer.h"
#include <GL/glew.h>

#include <vector>
#include <string>

//Menü egy állapotát nyilvántartó osztály
class MenuStateHandler 
{
public:
	MenuStateHandler();
	~MenuStateHandler();
	void Clean();

	void PreProcess();
	void Render(float pointed_element);

	void AddDecoratorTexture(float ndc_x, float ndc_y, float size_x, float size_y, GLuint texture);
	void AddDecoratorString(float ndc_x, float ndc_y, float size_x, const char* text);
	void AddButton(float ndc_x, float ndc_y, const char* name);
	void AddInputBox(const char* name);
	void BuildLayout();
	void UpdateInputBox(int id, const char* next_char, bool is_delete, bool is_cursor);

	std::vector<std::string> getInputStrings(int count);
protected:

	MenuRenderer* menuRenderer = new MenuRenderer(); //Szövegek textúrába rajzolása
	gVertexBuffer vb_decorator; //Nem klikkelhetõk modell adatok
	///gVertexBuffer vb_clickable; //Klikkelhetõ modell adatok
	gVertexBuffer vb_button; //Gomb modell adatai
	gVertexBuffer vb_inputbox; //InputBox modell adatai
	gShaderProgram sh_menu; //Menü shadere
	std::vector<GLuint> decoratorTextures; //Nem klikkelhetõ objektumok textúrája
	//std::vector<std::pair<std::string, GLuint>> clickableTextures; //Klikkelhetõ objektumok szövege és textúrája
	std::vector<std::pair<std::string, GLuint>> buttonTextures; //Gombok szövege és textúrája
	std::vector<std::pair<std::string, GLuint>> inputBoxTextures; //InputBoxok szövege és textúrája

	const size_t inputSizeLimit = 16; //Inputboxba maximum hány karaktert lehet írni
	const int preProcessOffset = 100; //3D pickinghez offset hogy ne 0-tól induljon

	float nextElementY = 0.4f; //Menü kövi elemét Y tengelyen hova rakjuk
};