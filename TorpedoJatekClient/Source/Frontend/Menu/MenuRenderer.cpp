
#include "MenuRenderer.h"

MenuRenderer::MenuRenderer()
{
	comicSans = TTF_OpenFont("Resources/comic.ttf", 72);
	if (!comicSans) {
		std::cout << "[TTF_OpenFont] Failed to load font: Comic Sans" << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "[TTF_OpenFont]", "Failed to load font: Comic Sans", nullptr);
	}
	candara = TTF_OpenFont("Resources/candara.ttf", 72);
	if (!candara) {
		std::cout << "[TTF_OpenFont] Failed to load font: Candara" << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "[TTF_OpenFont]", "Failed to load font: Candara", nullptr);
	}

	//TTF_SetFontWrappedAlign(comicSans, TTF_WRAPPED_ALIGN_CENTER);
	//TTF_SetFontWrappedAlign(candara, TTF_WRAPPED_ALIGN_CENTER);
	pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGB24);
}

MenuRenderer::~MenuRenderer()
{
	SDL_FreeSurface(textSurface);
	SDL_FreeFormat(pixelFormat);

	TTF_CloseFont(comicSans);
	TTF_CloseFont(candara);
}

//Solid módszerrel kirajzol szöveget egy SDL Surface-ba,majd átkonvertálja OPENGL-es textúrába
GLuint MenuRenderer::RenderTextSolid(const char* text)
{
	SDL_FreeSurface(textSurface);

	bgColor = SDL_Color{ 0,0,0,0 };
	bgColorNDC = glm::vec3(0);

	if (!(textSurface = TTF_RenderUTF8_Solid_Wrapped(comicSans, (text[0] ? text : " "), fgColor, 0))) {
		std::cout << "[RenderText_Solid] TTF Rendered text is NULL: " << SDL_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "[RenderText_Solid]", SDL_GetError(), nullptr);
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
	SDL_FreeSurface(textSurface);

	bgColor = { 255,0,0,0 };
	bgColorNDC.r = (bgColor.r != 0 ? (1.0f / bgColor.r) * 255 : 0);
	bgColorNDC.g = (bgColor.g != 0 ? (1.0f / bgColor.g) * 255 : 0);
	bgColorNDC.b = (bgColor.b != 0 ? (1.0f / bgColor.b) * 255 : 0);

	if (!(textSurface = TTF_RenderUTF8_Shaded_Wrapped(comicSans, (text[0] ? text : " "), fgColor, bgColor, 0))) {
		std::cout << "[RenderText_Shaded] TTF Rendered text is NULL: " << SDL_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "[RenderText_Shaded]", SDL_GetError(), nullptr);
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
