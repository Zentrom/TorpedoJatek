
#include "MenuStateHandler.h"

MenuStateHandler::MenuStateHandler()
{
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
	//vb_decorator.Clean();
	vb_clickable.Clean();
	sh_menu.Clean();

	delete menuRenderer;
	//for (GLuint& texture : decoratorTextures) {
	//	glDeleteTextures(1, &texture);
	//}
	for (std::pair<std::string, GLuint> &texture : clickableTextures) {
		glDeleteTextures(1, &texture.second);
	}
}

void MenuStateHandler::AddButton(const char* name)
{
	vb_clickable.AddData(0, 0.1f, nextElementY);
	vb_clickable.AddData(0, -0.1f, nextElementY);
	vb_clickable.AddData(0, -0.1f, nextElementY - 0.1f);
	vb_clickable.AddData(0, 0.1f, nextElementY - 0.1f);
	vb_clickable.AddData(1, 1, 0);
	vb_clickable.AddData(1, 0, 0);
	vb_clickable.AddData(1, 0, 1);
	vb_clickable.AddData(1, 1, 1);

	int indexCount = clickableTextures.size() * 4;
	vb_clickable.AddIndex(0 + indexCount, 1 + indexCount, 2 + indexCount);
	vb_clickable.AddIndex(0 + indexCount, 2 + indexCount, 3 + indexCount);

	clickableTextures.push_back(std::pair<std::string, GLuint>(std::string(name), menuRenderer->RenderTextSolid(name)));

	nextElementY -= 0.2f;

}

void MenuStateHandler::AddInputBox(const char* name)
{
	vb_clickable.AddData(0, 0.4f, nextElementY);
	vb_clickable.AddData(0, -0.4f, nextElementY);
	vb_clickable.AddData(0, -0.4f, nextElementY - 0.1f);
	vb_clickable.AddData(0, 0.4f, nextElementY - 0.1f);
	vb_clickable.AddData(1, 1, 0);
	vb_clickable.AddData(1, 0, 0);
	vb_clickable.AddData(1, 0, 1);
	vb_clickable.AddData(1, 1, 1);

	int indexCount = clickableTextures.size() * 4;
	vb_clickable.AddIndex(0 + indexCount, 1 + indexCount, 2 + indexCount);
	vb_clickable.AddIndex(0 + indexCount, 2 + indexCount, 3 + indexCount);

	clickableTextures.push_back(std::pair<std::string, GLuint>(std::string(name), menuRenderer->RenderTextSolid(name)));

	nextElementY -= 0.2f;
}

void MenuStateHandler::BuildLayout()
{
	vb_clickable.InitBuffers();
}

void MenuStateHandler::UpdateTexture(int id, const char* next_char, bool is_delete)
{
	int index = (id - preProcessOffset);
	if (!is_delete) {
		clickableTextures.at(index).first.append(next_char);
	}
	else {
		if (clickableTextures.at(index).first.size() != 0) {
			clickableTextures.at(index).first.pop_back();
		}
	}
	glDeleteTextures(1, &clickableTextures.at(index).second);
	clickableTextures.at(index).second = menuRenderer->RenderTextSolid(clickableTextures.at(index).first.c_str());
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
	sh_menu.SetUniform("is_PreProcess", false);
	sh_menu.Off();
}

//OPENGL-es végsõ kirajzolás
void MenuStateHandler::Render()
{
	//glDisable(GL_DEPTH_TEST);
	//sh_text.On();
	//sh_text.SetUniform("lineWidth", textLength / lineCount / textLengthDivisor);
	//sh_text.SetUniform("lineCount", static_cast<float>(lineCount) / lineHeightDivisor);
	//sh_text.SetUniform("bgColor", bgColorNDC);
	//sh_text.SetTexture("textTexture", 0, textTexture);
	sh_menu.On();

	vb_clickable.On();
	for (int i = 0; i < clickableTextures.size(); ++i) {
		sh_menu.SetTexture("textTexture", 0, clickableTextures.at(i).second);
		vb_clickable.DrawIndexed(GL_TRIANGLES, i * 6, 6);
	}
	vb_clickable.Off();

	sh_menu.Off();
	//sh_text.Off();
	//glEnable(GL_DEPTH_TEST);
}

std::vector<std::string> MenuStateHandler::getInputStrings(int count)
{
	std::vector<std::string> result;
	for (int i = 0; i < count; ++i) {
		result.push_back(clickableTextures.at(i).first);
	}
	return result;
}