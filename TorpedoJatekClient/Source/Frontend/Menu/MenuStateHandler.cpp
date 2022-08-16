
#include "MenuStateHandler.h"

MenuStateHandler::MenuStateHandler()
{
	//Decorator elemek
	vb_decorator.AddAttribute(0, 2); //pozíció
	vb_decorator.AddAttribute(2, 2); //textcoord
	//Gomb
	vb_button.AddAttribute(0, 2);
	vb_button.AddAttribute(1, 4);
	vb_button.AddAttribute(2, 2);
	//Inputmezõ
	vb_inputbox.AddAttribute(0, 2);
	vb_inputbox.AddAttribute(2, 2);

	sh_menu.AttachShader(GL_VERTEX_SHADER, "Shaders/menu.vert");
	sh_menu.AttachShader(GL_FRAGMENT_SHADER, "Shaders/menu.frag");
	sh_menu.BindAttribLoc(0, "vs_in_pos");
	sh_menu.BindAttribLoc(1, "vs_in_color");
	sh_menu.BindAttribLoc(2, "vs_in_tex");
	if (!sh_menu.LinkProgram()) {
		std::cout << "[Shader_Link]Error during Shader compilation: sh_menu" << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "[Shader_Link]", "Error during Shader compilation: sh_menu", nullptr);
	}
}

MenuStateHandler::~MenuStateHandler()
{
	vb_decorator.Clean();
	vb_button.Clean();
	vb_inputbox.Clean();
	sh_menu.Clean();
	
	delete menuRenderer;
	for (GLuint& texture : decoratorTextures) {
		glDeleteTextures(1, &texture);
	}
	for (std::pair<std::string, GLuint>& texture : buttonTextures) {
		glDeleteTextures(1, &texture.second);
	}
	for (std::pair<std::string, GLuint>& texture : inputBoxTextures) {
		glDeleteTextures(1, &texture.second);
	}
}

//Kiüríti az adatokat
void MenuStateHandler::Clean()
{
	vb_decorator.Clean();
	vb_button.Clean();
	vb_inputbox.Clean();

	for (std::pair<std::string, GLuint>& texture : buttonTextures) {
		glDeleteTextures(1, &texture.second);
	}
	buttonTextures.clear();
	for (std::pair<std::string, GLuint>& texture : inputBoxTextures) {
		glDeleteTextures(1, &texture.second);
	}
	buttonTextures.clear();
	nextElementY = 0.4f;
}

//Nem clickelhetõ textúrát helyez el a menübe
void MenuStateHandler::AddDecoratorTexture(float ndc_x, float ndc_y, float size_x, float size_y, GLuint texture)
{
	vb_decorator.AddData(0, ndc_x + (1.0f * size_x), ndc_y + (1.0f * size_y));
	vb_decorator.AddData(0, ndc_x - (1.0f * size_x), ndc_y + (1.0f * size_y));
	vb_decorator.AddData(0, ndc_x - (1.0f * size_x), ndc_y - (1.0f * size_y));
	vb_decorator.AddData(0, ndc_x + (1.0f * size_x), ndc_y - (1.0f * size_y));
	vb_decorator.AddData(2, 1, 0);
	vb_decorator.AddData(2, 0, 0);
	vb_decorator.AddData(2, 0, 1);
	vb_decorator.AddData(2, 1, 1);

	int indexCount = decoratorTextures.size() * 4;
	vb_decorator.AddIndex(0 + indexCount, 1 + indexCount, 2 + indexCount);
	vb_decorator.AddIndex(0 + indexCount, 2 + indexCount, 3 + indexCount);

	decoratorTextures.push_back(texture);
}

//Nem clickelhetõ szöveget helyez el a menübe
void MenuStateHandler::AddDecoratorString(float ndc_x, float ndc_y, float size_x, const char* text, float up_y)
{
	vb_decorator.AddData(0, ndc_x + (1.0f * size_x), ndc_y + up_y);
	vb_decorator.AddData(0, ndc_x - (1.0f * size_x), ndc_y + up_y);
	vb_decorator.AddData(0, ndc_x - (1.0f * size_x), ndc_y - 0.1f);
	vb_decorator.AddData(0, ndc_x + (1.0f * size_x), ndc_y - 0.1f);
	vb_decorator.AddData(2, 1, 0);
	vb_decorator.AddData(2, 0, 0);
	vb_decorator.AddData(2, 0, 1);
	vb_decorator.AddData(2, 1, 1);

	int indexCount = decoratorTextures.size() * 4;
	vb_decorator.AddIndex(0 + indexCount, 1 + indexCount, 2 + indexCount);
	vb_decorator.AddIndex(0 + indexCount, 2 + indexCount, 3 + indexCount);

	decoratorTextures.push_back(menuRenderer->RenderTextSolid(text));
}

//Gombot rak a menübe
void MenuStateHandler::AddButton(float ndc_x, float ndc_y, const char* name)
{
	float coordX = (ndc_x ? ndc_x : 0.0f);
	float coordY = (ndc_y ? ndc_y : nextElementY);

	vb_button.AddData(0, coordX + 0.1f, coordY);
	vb_button.AddData(0, coordX - 0.1f, coordY);
	vb_button.AddData(0, coordX - 0.1f, coordY - 0.1f);
	vb_button.AddData(0, coordX + 0.1f, coordY - 0.1f);
	vb_button.AddData(0, coordX - 0.2f, coordY - 0.05f);
	vb_button.AddData(0, coordX + 0.2f, coordY - 0.05f);
	vb_button.AddData(1, 0, 1.0f, 0, 1.0f);
	vb_button.AddData(1, 0, 1.0f, 0, 1.0f);
	vb_button.AddData(1, 0, 0, 1.0f, 1.0f);
	vb_button.AddData(1, 0, 0, 1.0f, 1.0f);
	vb_button.AddData(1, 1.0f, 0, 0, 1.0f);
	vb_button.AddData(1, 1.0f, 0, 0, 1.0f);
	vb_button.AddData(2, 1, 0);
	vb_button.AddData(2, 0, 0);
	vb_button.AddData(2, 0, 1);
	vb_button.AddData(2, 1, 1);
	vb_button.AddData(2, 0, 0);
	vb_button.AddData(2, 1, 0);

	int indexCount = buttonTextures.size() * 6;
	//Téglalap
	vb_button.AddIndex(0 + indexCount, 1 + indexCount, 2 + indexCount);
	vb_button.AddIndex(0 + indexCount, 2 + indexCount, 3 + indexCount);
	//Szélsõ háromszögek
	vb_button.AddIndex(4 + indexCount, 2 + indexCount, 1 + indexCount);
	vb_button.AddIndex(5 + indexCount, 0 + indexCount, 3 + indexCount);

	buttonTextures.push_back(std::pair<std::string, GLuint>(std::string(name), menuRenderer->RenderTextSolid(name)));

	if(!ndc_y) nextElementY -= 0.2f;
}

//Inputbox-ot rak a menübe
void MenuStateHandler::AddInputBox(const char* name)
{
	vb_inputbox.AddData(0, 0.4f, nextElementY);
	vb_inputbox.AddData(0, -0.2f, nextElementY);
	vb_inputbox.AddData(0, -0.2f, nextElementY - 0.1f);
	vb_inputbox.AddData(0, 0.4f, nextElementY - 0.1f);
	vb_inputbox.AddData(2, 1, 0);
	vb_inputbox.AddData(2, 0, 0);
	vb_inputbox.AddData(2, 0, 1);
	vb_inputbox.AddData(2, 1, 1);

	int indexCount = inputBoxTextures.size() * 4;
	vb_inputbox.AddIndex(0 + indexCount, 1 + indexCount, 2 + indexCount);
	vb_inputbox.AddIndex(0 + indexCount, 2 + indexCount, 3 + indexCount);

	std::string inputString;
	if (name) inputString = std::string(name);
	inputString.append(inputSizeLimit - inputString.size(), ' ');
	inputBoxTextures.push_back(std::pair<std::string, GLuint>(inputString, menuRenderer->RenderTextSolid(inputString.c_str())));

	nextElementY -= 0.2f;
}

//Ha már nem akarunk berakni több elemet akkor ezt meghívjuk
void MenuStateHandler::BuildLayout(bool has_inputBox)
{
	vb_decorator.InitBuffers();
	vb_button.InitBuffers();
	if (has_inputBox) vb_inputbox.InitBuffers();
}

//InputBox-ot frissíti
void MenuStateHandler::UpdateInputBox(int id, const char* next_char, bool is_delete, bool is_cursor)
{
	int index = (id - preProcessOffset);
	size_t firstSpace = inputBoxTextures.at(index).first.find(' ');
	char cursorChar = (is_cursor ? '|' : ' ');
	if (!is_delete) {
		if (firstSpace != std::string::npos) {
			if (next_char) {
				inputBoxTextures.at(index).first.replace(firstSpace, 1, next_char);
				//std::cout << inputBoxTextures.at(index).first << std::endl;
				//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "inputBox", inputBoxTextures.at(index).first.c_str(), nullptr);
				++firstSpace;
			}
			glDeleteTextures(1, &inputBoxTextures.at(index).second);
			std::string renderableString = std::string(inputBoxTextures.at(index).first);
			renderableString[firstSpace] = cursorChar;
			inputBoxTextures.at(index).second = menuRenderer->RenderTextSolid(renderableString.c_str());
		}
		return;
	}
	else {
		if (firstSpace == std::string::npos) {
			inputBoxTextures.at(index).first.back() = ' ';
		}
		else if (firstSpace != 0) {
			inputBoxTextures.at(index).first[firstSpace - 1] = ' ';
		}
	}
	glDeleteTextures(1, &inputBoxTextures.at(index).second);
	std::string renderableString = std::string(inputBoxTextures.at(index).first);
	firstSpace = inputBoxTextures.at(index).first.find(' ');
	renderableString[firstSpace] = cursorChar;
	inputBoxTextures.at(index).second = menuRenderer->RenderTextSolid(renderableString.c_str());
}

//3D picking a menüelemek clickelésére
void MenuStateHandler::PreProcess()
{
	int pickingIndex = preProcessOffset;
	sh_menu.On();
	sh_menu.SetUniform("is_PreProcess", true);
	vb_inputbox.On();
	for (unsigned int i = 0; i < inputBoxTextures.size(); ++i) {
		sh_menu.SetUniform("clickableIndex", pickingIndex++);
		vb_inputbox.DrawIndexed(GL_TRIANGLES, i * 6, 6);
	}
	vb_inputbox.Off();
	vb_button.On();
	for (unsigned int i = 0; i < buttonTextures.size(); ++i) {
		sh_menu.SetUniform("clickableIndex", pickingIndex++);
		vb_button.DrawIndexed(GL_TRIANGLES, i * 12, 12);
	}
	vb_button.Off();
	sh_menu.SetUniform("clickableIndex", preProcessOffset * 3);
	sh_menu.SetUniform("is_PreProcess", false);
	sh_menu.Off();
}

//Menüállapot kirajzolása
void MenuStateHandler::Render(float pointed_element)
{
	sh_menu.On();

	sh_menu.SetUniform("is_Decorator", true);
	vb_decorator.On();
	for (unsigned int i = 0; i < decoratorTextures.size(); ++i) {
		sh_menu.SetTexture("textTexture", 0, decoratorTextures.at(i));
		vb_decorator.DrawIndexed(GL_TRIANGLES, i * 6, 6);
	}
	vb_decorator.Off();
	sh_menu.SetUniform("is_Decorator", false);

	int pickingIndex = preProcessOffset;
	sh_menu.SetUniform("read_index", static_cast<int>(pointed_element));
	vb_inputbox.On();
	for (unsigned int i = 0; i < inputBoxTextures.size(); ++i) {
		sh_menu.SetTexture("textTexture", 0, inputBoxTextures.at(i).second);
		sh_menu.SetUniform("clickableIndex", pickingIndex++);
		vb_inputbox.DrawIndexed(GL_TRIANGLES, i * 6, 6);
	}
	vb_inputbox.Off();
	vb_button.On();
	for (unsigned int i = 0; i < buttonTextures.size(); ++i) {
		sh_menu.SetTexture("textTexture", 0, buttonTextures.at(i).second);
		sh_menu.SetUniform("clickableIndex", pickingIndex++);
		vb_button.DrawIndexed(GL_TRIANGLES, i * 12, 12);
	}
	vb_button.Off();
	
	sh_menu.Off();
}

//Inputbox szövegeit adja vissza
std::vector<std::string> MenuStateHandler::getInputStrings(int count)
{
	std::vector<std::string> result;
	for (int i = 0; i < count; ++i) {
		result.push_back(inputBoxTextures.at(i).first);
	}
	return result;
}