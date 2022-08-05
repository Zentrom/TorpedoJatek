#pragma once

#include "../../../Utility/gVertexBuffer.h"
#include "../../../Utility/gShaderProgram.h"

#include <SDL_ttf.h>
#include <GL/glew.h>

#include <vector>
#include <iostream>

//Menü kirajzoló osztálya
class MenuRenderer {
public:
	MenuRenderer();
	~MenuRenderer();

	GLuint RenderTextSolid(const char* text);
	GLuint RenderTextShaded(const char* text);
private:
	TTF_Font* comicSans; //Betûtípus adat
	TTF_Font* candara; //Betûtípus adat

	SDL_PixelFormat* pixelFormat; //Pixelformátum amelybe konvertáljuk az SDL Surface-t,hogy OPENGL ismerje
	SDL_Surface* textSurface = nullptr; //SDL_ttf ebbe a felületbe tudja a szöveget kirajzolni
	GLuint textTexture = 0; //Szövegmezõ OPENGL-es textúrája
	SDL_Color fgColor = { 255, 255, 255 }; //Szöveg színe
	SDL_Color bgColor = { 255, 0, 0 }; //Shaded rajzoláshoz a háttérszín(Fragment shader nagyrészét eldobja)
	glm::vec3 bgColorNDC = glm::vec3(0.0f); //Fragment shadernek,hogy ott 0-1 között legyen az érték
};