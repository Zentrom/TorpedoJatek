
#include "TextHandler.h"

TextHandler::TextHandler() 
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

	vb_message.AddAttribute(0, 2); //pozíció
	vb_message.AddAttribute(1, 2); //textcoord

	vb_message.AddData(0, 0.1f, 0.0f);
	vb_message.AddData(0,-0.1f, 0.0f);
	vb_message.AddData(0,-0.1f,-0.1f); 
	vb_message.AddData(0, 0.1f,-0.1f); //glBufferSubData()-val lehet ezt felülirni VAGY transformolni betüméret alapján vertex shaderbe

	vb_message.AddData(1, 1, 0);
	vb_message.AddData(1, 0, 0);
	vb_message.AddData(1, 0, 1);
	vb_message.AddData(1, 1, 1);

	vb_message.AddIndex(0, 1, 2);
	vb_message.AddIndex(0, 2, 3);

	vb_message.InitBuffers();

	sh_text.AttachShader(GL_VERTEX_SHADER, "Shaders/text.vert");
	sh_text.AttachShader(GL_FRAGMENT_SHADER, "Shaders/text.frag");
	sh_text.BindAttribLoc(0, "vs_in_pos");
	sh_text.BindAttribLoc(1, "vs_in_tex");
	if (!sh_text.LinkProgram()) {
		std::cout << "[Shader_Link]Error during Shader compilation: sh_text" << std::endl;
	}
}

TextHandler::~TextHandler()
{
	glDeleteTextures(1, &textTexture);
	SDL_FreeSurface(textSurface);
	SDL_FreeFormat(pixelFormat);

	vb_message.Clean();
	sh_text.Clean();

	TTF_CloseFont(comicSans);
	TTF_CloseFont(candara);
}

//Szöveg hozzáfûzése a stringstreamhez
TextHandler& TextHandler::operator<<(const char* text)
{
	textStream << text;
	CalcMetrics();
	RenderTextSolid(textStream.str().c_str());
	return *this;
}

//Szám hozzáfûzése
TextHandler& TextHandler::operator<<(int num)
{
	textStream << num;
	CalcMetrics();
	RenderTextSolid(textStream.str().c_str());
	return *this;
}

//Karakter hozzáfûzése
TextHandler& TextHandler::operator<<(char text)
{
	textStream << text;
	CalcMetrics();
	RenderTextSolid(textStream.str().c_str());
	return *this;
}

//Új szöveg kezdése
TextHandler& TextHandler::operator<(const char* text)
{
	textStream.str(std::string());
	textStream << text;
	CalcMetrics();
	RenderTextSolid(textStream.str().c_str());
	return *this;
}

//Új szöveg kezdése egy számmal
TextHandler& TextHandler::operator<(int num)
{
	textStream.str(std::string());
	textStream << num;
	CalcMetrics();
	RenderTextSolid(textStream.str().c_str());
	return *this;
}

TextHandler& TextHandler::operator<(char text)
{
	textStream.str(std::string());
	textStream << text;
	CalcMetrics();
	RenderTextSolid(textStream.str().c_str());
	return *this;
}

void TextHandler::Clear()
{
	textStream.str(std::string());
}

//Szöveghossz és sorok száma alapján kiszámolja a szövegmezõ méretét
void TextHandler::CalcMetrics()
{
	int* wi = new int;
	int* hi = new int;
	TTF_SizeUTF8(comicSans, textStream.str().c_str(), wi, hi);
	//std::cout << *wi << " --- " << *hi << std::endl;
	textLength = static_cast<float>(*wi);

	lineCount = 1;
	for (char ch : textStream.str()) {
		if (ch == '\n') ++lineCount;
	}
	//std::cout << "szam: " << lineCount << std::endl;
	delete wi;
	delete hi;
}

//OPENGL-es végsõ kirajzolás
void TextHandler::Render()
{
	if (textTexture) {
		glDisable(GL_DEPTH_TEST);
		sh_text.On();
		sh_text.SetUniform("lineWidth", textLength / lineCount / textLengthDivisor);
		sh_text.SetUniform("lineCount", static_cast<float>(lineCount) / lineHeightDivisor);

		sh_text.SetUniform("bgColor", bgColorNDC);
		sh_text.SetTexture("textTexture", 0, textTexture);
		vb_message.On();
		vb_message.DrawIndexed(GL_TRIANGLES, 0, 6);
		vb_message.Off();
		sh_text.Off();
		glEnable(GL_DEPTH_TEST);
	}
}

//Solid módszerrel kirajzol szöveget egy SDL Surface-ba,majd átkonvertálja OPENGL-es textúrába
GLuint TextHandler::RenderTextSolid(const char* text)
{
	glDeleteTextures(1, &textTexture);
	SDL_FreeSurface(textSurface);

	bgColor = SDL_Color{ 0,0,0,0 };
	bgColorNDC = glm::vec3(0);

	if (!(textSurface = TTF_RenderUTF8_Solid_Wrapped(comicSans, text, fgColor, 0))) {
		std::cout << "[RenderText_Solid] TTF Rendered text is NULL: " << SDL_GetError() << std::endl;
	}

	//std::cout << "texturesize: " << textSurface->w << " " << textSurface->h << std::endl;
	//std::cout << "textsurfaceformat " << SDL_GetPixelFormatName(textSurface->format->format) << " " << +textSurface->format->BitsPerPixel << " " << +textSurface->format->BytesPerPixel << std::endl;

	glGenTextures(1, &textTexture);
	glBindTexture(GL_TEXTURE_2D, textTexture);

	SDL_Surface* convertedSurface = SDL_ConvertSurface(textSurface, pixelFormat, 0);

	//std::cout << "covertedSurface " << SDL_GetPixelFormatName(convertedSurface->format->format) << " " << +convertedSurface->format->BitsPerPixel << " " << +convertedSurface->format->BytesPerPixel << std::endl;

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
GLuint TextHandler::RenderTextShaded(const char* text)
{
	glDeleteTextures(1, &textTexture);
	SDL_FreeSurface(textSurface);

	bgColor = { 255,0,0,0 };
	bgColorNDC.r = (bgColor.r != 0 ? (1.0f / bgColor.r) * 255 : 0);
	bgColorNDC.g = (bgColor.g != 0 ? (1.0f / bgColor.g) * 255 : 0);
	bgColorNDC.b = (bgColor.b != 0 ? (1.0f / bgColor.b) * 255 : 0);

	if (!(textSurface = TTF_RenderUTF8_Shaded_Wrapped(comicSans, text, fgColor, bgColor, 0))) {
		std::cout << "[RenderText_Shaded] TTF Rendered text is NULL: " << SDL_GetError() << std::endl;
	}
	
	//std::cout << "texturesize " << textSurface->w << " " << textSurface->h << std::endl;
	//std::cout << "textsurfaceformat " << SDL_GetPixelFormatName(textSurface->format->format) << " " << +textSurface->format->BitsPerPixel << " " << +textSurface->format->BytesPerPixel << std::endl;
	
	glGenTextures(1, &textTexture);
	glBindTexture(GL_TEXTURE_2D, textTexture);
	
	SDL_Surface* convertedSurface = SDL_ConvertSurface(textSurface, pixelFormat, 0);
	
	//std::cout << "convertedSurface " << SDL_GetPixelFormatName(convertedSurface->format->format) << " " << +convertedSurface->format->BitsPerPixel << " " << +convertedSurface->format->BytesPerPixel << std::endl;
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, convertedSurface->w, convertedSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, convertedSurface->pixels);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(convertedSurface);

	return textTexture;
}
