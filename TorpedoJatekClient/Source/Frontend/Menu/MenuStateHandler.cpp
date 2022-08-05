
#include "MenuStateHandler.h"

MenuStateHandler::MenuStateHandler()
{
	vb_decorator.AddAttribute(0, 2); //pozíció
	vb_decorator.AddAttribute(1, 2); //textcoord

	vb_clickable.AddAttribute(0, 2); //pozíció
	vb_clickable.AddAttribute(1, 2); //textcoord

	sh_menu.AttachShader(GL_VERTEX_SHADER, "Shaders/menu.vert");
	sh_menu.AttachShader(GL_FRAGMENT_SHADER, "Shaders/menu.frag");
	sh_menu.BindAttribLoc(0, "vs_in_pos");
	sh_menu.BindAttribLoc(1, "vs_in_tex");
	if (!sh_menu.LinkProgram()) {
		std::cout << "[Shader_Link]Error during Shader compilation: sh_menu" << std::endl;
	}
}

MenuStateHandler::~MenuStateHandler()
{
	vb_decorator.Clean();
	vb_clickable.Clean();
	sh_menu.Clean();

	delete menuRenderer;
	for (GLuint& texture : decoratorTextures) {
		glDeleteTextures(1, &texture);
	}
	for (std::pair<std::string, GLuint> &texture : clickableTextures) {
		glDeleteTextures(1, &texture.second);
	}
}

//Kiüríti az adatokat
void MenuStateHandler::Clean()
{
	vb_clickable.Clean();
	for (std::pair<std::string, GLuint>& texture : clickableTextures) {
		glDeleteTextures(1, &texture.second);
	}
	clickableTextures.clear();
	nextElementY = 0.4f;
}

//Nem clickelhetõ textúrát helyez el a menübe
void MenuStateHandler::AddDecoratorTexture(float ndc_x, float ndc_y, float size_x, float size_y, GLuint texture)
{
	vb_decorator.AddData(0, ndc_x + (1.0f * size_x), ndc_y + (1.0f * size_y));
	vb_decorator.AddData(0, ndc_x - (1.0f * size_x), ndc_y + (1.0f * size_y));
	vb_decorator.AddData(0, ndc_x - (1.0f * size_x), ndc_y - (1.0f * size_y));
	vb_decorator.AddData(0, ndc_x + (1.0f * size_x), ndc_y - (1.0f * size_y));
	vb_decorator.AddData(1, 1, 0);
	vb_decorator.AddData(1, 0, 0);
	vb_decorator.AddData(1, 0, 1);
	vb_decorator.AddData(1, 1, 1);

	int indexCount = decoratorTextures.size() * 4;
	vb_decorator.AddIndex(0 + indexCount, 1 + indexCount, 2 + indexCount);
	vb_decorator.AddIndex(0 + indexCount, 2 + indexCount, 3 + indexCount);

	decoratorTextures.push_back(texture);
}

//Nem clickelhetõ szöveget helyez el a menübe
void MenuStateHandler::AddDecoratorString(float ndc_x, float ndc_y, float size_x, const char* text)
{
	vb_decorator.AddData(0, ndc_x + (1.0f * size_x), ndc_y);
	vb_decorator.AddData(0, ndc_x - (1.0f * size_x), ndc_y);
	vb_decorator.AddData(0, ndc_x - (1.0f * size_x), ndc_y - 0.1f);
	vb_decorator.AddData(0, ndc_x + (1.0f * size_x), ndc_y - 0.1f);
	vb_decorator.AddData(1, 1, 0);
	vb_decorator.AddData(1, 0, 0);
	vb_decorator.AddData(1, 0, 1);
	vb_decorator.AddData(1, 1, 1);

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

	vb_clickable.AddData(0, coordX + 0.1f, coordY);
	vb_clickable.AddData(0, coordX - 0.1f, coordY);
	vb_clickable.AddData(0, coordX - 0.1f, coordY - 0.1f);
	vb_clickable.AddData(0, coordX + 0.1f, coordY - 0.1f);
	vb_clickable.AddData(1, 1, 0);
	vb_clickable.AddData(1, 0, 0);
	vb_clickable.AddData(1, 0, 1);
	vb_clickable.AddData(1, 1, 1);

	int indexCount = clickableTextures.size() * 4;
	vb_clickable.AddIndex(0 + indexCount, 1 + indexCount, 2 + indexCount);
	vb_clickable.AddIndex(0 + indexCount, 2 + indexCount, 3 + indexCount);

	clickableTextures.push_back(std::pair<std::string, GLuint>(std::string(name), menuRenderer->RenderTextSolid(name)));

	if(!ndc_y) nextElementY -= 0.2f;
}

//Inputbox-ot rak a menübe
void MenuStateHandler::AddInputBox(const char* name)
{
	vb_clickable.AddData(0, 0.4f, nextElementY);
	vb_clickable.AddData(0, -0.2f, nextElementY);
	vb_clickable.AddData(0, -0.2f, nextElementY - 0.1f);
	vb_clickable.AddData(0, 0.4f, nextElementY - 0.1f);
	vb_clickable.AddData(1, 1, 0);
	vb_clickable.AddData(1, 0, 0);
	vb_clickable.AddData(1, 0, 1);
	vb_clickable.AddData(1, 1, 1);

	int indexCount = clickableTextures.size() * 4;
	vb_clickable.AddIndex(0 + indexCount, 1 + indexCount, 2 + indexCount);
	vb_clickable.AddIndex(0 + indexCount, 2 + indexCount, 3 + indexCount);

	std::string inputString;
	if (name) inputString = std::string(name);
	inputString.append(inputSizeLimit - inputString.size(), ' ');
	clickableTextures.push_back(std::pair<std::string, GLuint>(inputString, menuRenderer->RenderTextSolid(inputString.c_str())));

	nextElementY -= 0.2f;
}

//Ha már nem akarunk berakni több elemet akkor ezt meghívjuk
void MenuStateHandler::BuildLayout()
{
	vb_decorator.InitBuffers();
	vb_clickable.InitBuffers();
}

//InputBox-ot frissíti
void MenuStateHandler::UpdateInputBox(int id, const char* next_char, bool is_delete, bool is_cursor)
{
	int index = (id - preProcessOffset);
	size_t firstSpace = clickableTextures.at(index).first.find(' ');
	char cursorChar = (is_cursor ? '|' : ' ');
	if (!is_delete) {
		if (firstSpace != std::string::npos) {
			if (next_char) {
				clickableTextures.at(index).first.replace(firstSpace, 1, next_char);
				++firstSpace;
			}
			glDeleteTextures(1, &clickableTextures.at(index).second);
			std::string renderableString = std::string(clickableTextures.at(index).first);
			renderableString[firstSpace] = cursorChar;
			clickableTextures.at(index).second = menuRenderer->RenderTextSolid(renderableString.c_str());
		}
		return;
	}
	else {
		if (firstSpace == std::string::npos) {
			clickableTextures.at(index).first.back() = ' ';
		}
		if (firstSpace != 0) {
			clickableTextures.at(index).first[firstSpace - 1] = ' ';
		}
	}
	glDeleteTextures(1, &clickableTextures.at(index).second);
	std::string renderableString = std::string(clickableTextures.at(index).first);
	firstSpace = clickableTextures.at(index).first.find(' ');
	renderableString[firstSpace] = cursorChar;
	clickableTextures.at(index).second = menuRenderer->RenderTextSolid(renderableString.c_str());
}

//3D picking a menüelemek clickelésére
void MenuStateHandler::PreProcess()
{
	sh_menu.On();
	sh_menu.SetUniform("is_PreProcess", true);
	vb_clickable.On();
	for (int i = 0; i < clickableTextures.size(); ++i) {
		//sh_menu.SetTexture("textTexture", 0, menuButtonTextures.at(i));
		sh_menu.SetUniform("clickableIndex", i + preProcessOffset);
		vb_clickable.DrawIndexed(GL_TRIANGLES, i * 6, 6);
	}
	vb_clickable.Off();
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
	for (int i = 0; i < decoratorTextures.size(); ++i) {
		sh_menu.SetTexture("textTexture", 0, decoratorTextures.at(i));
		vb_decorator.DrawIndexed(GL_TRIANGLES, i * 6, 6);
	}
	vb_decorator.Off();
	sh_menu.SetUniform("is_Decorator", false);

	vb_clickable.On();
	sh_menu.SetUniform("read_index", static_cast<int>(pointed_element));
	for (int i = 0; i < clickableTextures.size(); ++i) {
		sh_menu.SetTexture("textTexture", 0, clickableTextures.at(i).second);
		sh_menu.SetUniform("clickableIndex", i + preProcessOffset);
		vb_clickable.DrawIndexed(GL_TRIANGLES, i * 6, 6);
	}
	vb_clickable.Off();

	sh_menu.Off();
}

//Inputbox szövegeit adja vissza
std::vector<std::string> MenuStateHandler::getInputStrings(int count)
{
	std::vector<std::string> result;
	for (int i = 0; i < count; ++i) {
		result.push_back(clickableTextures.at(i).first);
	}
	return result;
}