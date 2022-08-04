
#include "MenuRenderer.h"

MenuRenderer::MenuRenderer()
{
	comicSans = TTF_OpenFont("Resources/comic.ttf", 72);
	if (!comicSans) {
		std::cout << "[TTF_OpenFont] Failed to load font: Comic Sans" << std::endl;
	}
	candara = TTF_OpenFont("Resources/candara.ttf", 72);
	if (!candara) {
		std::cout << "[TTF_OpenFont] Failed to load font: Candara" << std::endl;
	}

	//TTF_SetFontWrappedAlign(comicSans, TTF_WRAPPED_ALIGN_CENTER);
	//TTF_SetFontWrappedAlign(candara, TTF_WRAPPED_ALIGN_CENTER);
	pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGB24);

	//vb_menu.AddAttribute(0, 2); //pozíció
	//vb_menu.AddAttribute(1, 2); //textcoord
	//
	////Play gomb
	//vb_menu.AddData(0, 0.1f, 0.0f);
	//vb_menu.AddData(0, -0.1f, 0.0f);
	//vb_menu.AddData(0, -0.1f, -0.1f);
	//vb_menu.AddData(0, 0.1f, -0.1f);
	//vb_menu.AddData(1, 1, 0);
	//vb_menu.AddData(1, 0, 0);
	//vb_menu.AddData(1, 0, 1);
	//vb_menu.AddData(1, 1, 1);
	//
	////Quit gomb
	//vb_menu.AddData(0, 0.1f, -0.2f);
	//vb_menu.AddData(0, -0.1f, -0.2f);
	//vb_menu.AddData(0, -0.1f, -0.3f);
	//vb_menu.AddData(0, 0.1f, -0.3f);
	//vb_menu.AddData(1, 1, 0);
	//vb_menu.AddData(1, 0, 0);
	//vb_menu.AddData(1, 0, 1);
	//vb_menu.AddData(1, 1, 1);
	//
	//vb_menu.AddIndex(0, 1, 2);
	//vb_menu.AddIndex(0, 2, 3);
	//
	//vb_menu.AddIndex(4, 5, 6);
	//vb_menu.AddIndex(4, 6, 7);
	//
	//vb_menu.InitBuffers();

	

	//RenderTextSolid("Play");
	//RenderTextSolid("Quit");
}

MenuRenderer::~MenuRenderer()
{
	//for (GLuint &texture : menuButtonTextures) {
	//	glDeleteTextures(1, &texture);
	//}

	SDL_FreeSurface(textSurface);
	SDL_FreeFormat(pixelFormat);

	TTF_CloseFont(comicSans);
	TTF_CloseFont(candara);
}

////Szöveg hozzáfûzése a stringstreamhez
//MenuRenderer& MenuRenderer::operator<<(const char* text)
//{
//	textStream << text;
//	CalcMetrics();
//	RenderTextSolid(textStream.str().c_str());
//	return *this;
//}
//
////Szám hozzáfûzése
//MenuRenderer& MenuRenderer::operator<<(int num)
//{
//	textStream << num;
//	CalcMetrics();
//	RenderTextSolid(textStream.str().c_str());
//	return *this;
//}
//
////Karakter hozzáfûzése
//MenuRenderer& MenuRenderer::operator<<(char text)
//{
//	textStream << text;
//	CalcMetrics();
//	RenderTextSolid(textStream.str().c_str());
//	return *this;
//}
//
////Új szöveg kezdése
//MenuRenderer& MenuRenderer::operator<(const char* text)
//{
//	textStream.str(std::string());
//	textStream << text;
//	CalcMetrics();
//	RenderTextSolid(textStream.str().c_str());
//	return *this;
//}
//
////Új szöveg kezdése egy számmal
//MenuRenderer& MenuRenderer::operator<(int num)
//{
//	textStream.str(std::string());
//	textStream << num;
//	CalcMetrics();
//	RenderTextSolid(textStream.str().c_str());
//	return *this;
//}
//
//MenuRenderer& MenuRenderer::operator<(char text)
//{
//	textStream.str(std::string());
//	textStream << text;
//	CalcMetrics();
//	RenderTextSolid(textStream.str().c_str());
//	return *this;
//}
//
//void MenuRenderer::Clear()
//{
//	textStream.str(std::string());
//}

//Szöveghossz és sorok száma alapján kiszámolja a szövegmezõ méretét
//void MenuRenderer::CalcMetrics()
//{
//	int* wi = new int;
//	int* hi = new int;
//	TTF_SizeUTF8(comicSans, textStream.str().c_str(), wi, hi);
//	//std::cout << *wi << " --- " << *hi << std::endl;
//	textLength = static_cast<float>(*wi);
//
//	lineCount = 1;
//	for (char ch : textStream.str()) {
//		if (ch == '\n') ++lineCount;
//	}
//	//std::cout << "szam: " << lineCount << std::endl;
//	delete wi;
//	delete hi;
//}

//Solid módszerrel kirajzol szöveget egy SDL Surface-ba,majd átkonvertálja OPENGL-es textúrába
GLuint MenuRenderer::RenderTextSolid(const char* text)
{
	//if (textTexture) glDeleteTextures(1, &textTexture);
	//SDL_FreeSurface(textSurface);
	//menuButtonTextures.push_back(GLuint());

	bgColor = SDL_Color{ 0,0,0,0 };
	bgColorNDC = glm::vec3(0);

	if (!(textSurface = TTF_RenderUTF8_Solid_Wrapped(comicSans, (text[0] ? text : " "), fgColor, 0))) {
		std::cout << "[RenderText_Solid] TTF Rendered text is NULL: " << SDL_GetError() << std::endl;
	}

	glGenTextures(1, &textTexture);
	glBindTexture(GL_TEXTURE_2D, textTexture);

	SDL_Surface* convertedSurface = SDL_ConvertSurface(textSurface, pixelFormat, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, convertedSurface->w, convertedSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, convertedSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(convertedSurface);

	return textTexture;
}

//Shaded módszerrel kirajzol szöveget egy SDL Surface-ba,majd átkonvertálja OPENGL-es textúrába
GLuint MenuRenderer::RenderTextShaded(const char* text)
{
	//if (textTexture) glDeleteTextures(1, &textTexture);
	//SDL_FreeSurface(textSurface);
	//menuButtonTextures.push_back(GLuint());

	bgColor = { 255,0,0,0 };
	bgColorNDC.r = (bgColor.r != 0 ? (1.0f / bgColor.r) * 255 : 0);
	bgColorNDC.g = (bgColor.g != 0 ? (1.0f / bgColor.g) * 255 : 0);
	bgColorNDC.b = (bgColor.b != 0 ? (1.0f / bgColor.b) * 255 : 0);

	if (!(textSurface = TTF_RenderUTF8_Shaded_Wrapped(comicSans, (text[0] ? text : " "), fgColor, bgColor, 0))) {
		std::cout << "[RenderText_Shaded] TTF Rendered text is NULL: " << SDL_GetError() << std::endl;
	}

	glGenTextures(1, &textTexture);
	glBindTexture(GL_TEXTURE_2D, textTexture);

	SDL_Surface* convertedSurface = SDL_ConvertSurface(textSurface, pixelFormat, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, convertedSurface->w, convertedSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, convertedSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(convertedSurface);

	return textTexture;
}
