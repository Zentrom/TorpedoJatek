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

	//void PreProcess();
	//void Render();

	//MenuRenderer& operator<<(const char* text);
	//MenuRenderer& operator<<(char text);
	//MenuRenderer& operator<<(int num);
	//MenuRenderer& operator<(const char* text);
	//MenuRenderer& operator<(char text);
	//MenuRenderer& operator<(int num);
	//void Clear();
	GLuint RenderTextSolid(const char* text);
	GLuint RenderTextShaded(const char* text);
private:

	
	//void CalcMetrics();

	TTF_Font* comicSans; //Betûtípus adat
	TTF_Font* candara; //Betûtípus adat
	//gShaderProgram sh_text; //Szöveg shadere
	
	//gVertexBuffer vb_menu; //Menü elemeinek modell adatai

	//const float textLengthDivisor = 200.0f; //Szövegmezõt mennyire sûrítjük
	//const float lineHeightDivisor = 1.5f; //Magasság sûrítése

	SDL_PixelFormat* pixelFormat; //Pixelformátum amelybe konvertáljuk az SDL Surface-t,hogy OPENGL ismerje
	SDL_Surface* textSurface = nullptr; //SDL_ttf ebbe a felületbe tudja a szöveget kirajzolni
	GLuint textTexture = 0; //Szövegmezõ OPENGL-es textúrája
	//std::vector<GLuint> menuButtonTextures; //Menü gombjainak textúrája
	SDL_Color fgColor = { 255, 255, 255 }; //Szöveg színe
	SDL_Color bgColor = { 255, 0, 0 }; //Shaded rajzoláshoz a háttérszín(Fragment shader nagyrészét eldobja)
	glm::vec3 bgColorNDC = glm::vec3(0.0f); //Fragment shadernek,hogy ott 0-1 között legyen az érték

	//const int preProcessOffset = 100;

	//std::stringstream textStream; //Kirajzolandó szövegnek a gyûjtõje
	//int lineCount = 1; //Sorok száma
	//float textLength = 0; //Szöveg teljes szélessége
};