
#include "OptionHandler.h"

OptionHandler::OptionHandler(std::map<std::string, int>& options) : optionsRef(options)
{
	vb_clickables.AddAttribute(0, 2);
	vb_clickables.AddAttribute(1, 4);
	vb_clickables.AddAttribute(2, 2);

	sh_options.AttachShader(GL_VERTEX_SHADER, "Shaders/options.vert");
	sh_options.AttachShader(GL_FRAGMENT_SHADER, "Shaders/options.frag");
	sh_options.BindAttribLoc(0, "vs_in_pos");
	sh_options.BindAttribLoc(1, "vs_in_color");
	sh_options.BindAttribLoc(2, "vs_in_tex");
	if (!sh_options.LinkProgram()) {
		std::cout << "[Shader_Link]Error during Shader compilation: sh_options" << std::endl;
	}
	SDL_DisplayMode mode;
	for (int i = 0; i < SDL_GetNumDisplayModes(0); ++i) {
		if (SDL_GetDisplayMode(0, i, &mode) != 0) {
			std::cout << "[OptionHandler] SDL_GetDisplayMode failed: " << SDL_GetError() << std::endl;
		}
		else {
			//std::cout << "SDL_GetALLDisplayMode: " << SDL_BITSPERPIXEL(mode.format) << " bpp " << mode.w << 'x' << mode.h << ' ' << mode.refresh_rate << "Hz" << std::endl;
			
			bool found = false;
			for (std::pair<int, int>& dismode : displayModes) {
				if (dismode.first == mode.w && dismode.second == mode.h) {
					found = true;
					break;
				}
			}
			if (!found) displayModes.push_back(std::pair<int, int>(mode.w, mode.h));
		}
	}

	displayModeIndex = std::find(displayModes.begin(), displayModes.end(), std::pair<int, int>(options["ResolutionWidth"], options["ResolutionHeight"]));

	//for (std::pair<int, int>& dismode : displayModes) {
	//	std::cout << dismode.first << " x " << dismode.second << std::endl;
	//}

}

OptionHandler::~OptionHandler()
{
	vb_clickables.Clean();
	sh_options.Clean();
}

void OptionHandler::PreProcess()
{
	sh_menu.On();
	sh_menu.SetUniform("is_PreProcess", true);
	int pickingIndex = preProcessOffset;
	vb_button.On();
	for (int i = 0; i < buttonTextures.size(); ++i) {
		sh_menu.SetUniform("clickableIndex", pickingIndex++);
		vb_button.DrawIndexed(GL_TRIANGLES, i * 12, 12);
	}
	vb_button.Off();
	vb_clickables.On();
	sh_menu.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 0, 3); //nyil
	sh_menu.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 3, 3); //nyil
	sh_menu.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 6, 6); //fullscreen
	sh_menu.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 12, 6); //vsync
	sh_menu.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 18, 6); //audioslider
	sh_menu.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 24, 6); //audioslider
	vb_clickables.Off();
	sh_menu.SetUniform("clickableIndex", preProcessOffset * 3);
	sh_menu.SetUniform("is_PreProcess", false);
	sh_menu.Off();
}

void OptionHandler::Render(float pointed_element)
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

	int pickingIndex = preProcessOffset;
	sh_menu.SetUniform("read_index", static_cast<int>(pointed_element));
	vb_button.On();
	for (int i = 0; i < buttonTextures.size(); ++i) {
		sh_menu.SetTexture("textTexture", 0, buttonTextures.at(i).second);
		sh_menu.SetUniform("clickableIndex", pickingIndex++);
		vb_button.DrawIndexed(GL_TRIANGLES, i * 12, 12);
	}
	vb_button.Off();
	sh_menu.Off();

	sh_options.On();
	sh_options.SetUniform("read_index", static_cast<int>(pointed_element));
	vb_clickables.On();
	sh_options.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 0, 3); //nyil
	sh_options.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 3, 3); //nyil
	sh_options.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 6, 6); //fullscreen
	sh_menu.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 12, 6); //fullscreen
	sh_options.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 18, 6); //audioslider
	sh_options.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 24, 6); //audioslider
	vb_clickables.Off();

	sh_options.Off();
}

void OptionHandler::AddClickableOptions()
{
	//Felbontás
	vb_decorator.AddData(0, -0.1f, 0.2f);
	vb_decorator.AddData(0, -0.3f, 0.2f);
	vb_decorator.AddData(0, -0.3f, 0.1f);
	vb_decorator.AddData(0, -0.1f, 0.1f);
	vb_decorator.AddData(2, 1, 0);
	vb_decorator.AddData(2, 0, 0);
	vb_decorator.AddData(2, 0, 1);
	vb_decorator.AddData(2, 1, 1);
	int indexCount = decoratorTextures.size() * 4;
	vb_decorator.AddIndex(0 + indexCount, 1 + indexCount, 2 + indexCount);
	vb_decorator.AddIndex(0 + indexCount, 2 + indexCount, 3 + indexCount);
	std::string resolution = std::string(std::to_string(optionsRef["ResolutionWidth"]) + " x " + std::to_string(optionsRef["ResolutionHeight"]));
	decoratorTextures.push_back(menuRenderer->RenderTextShaded(resolution.c_str()));

	//Felbontás - Balnyíl
	vb_clickables.AddData(0, -0.3f, 0.2f);
	vb_clickables.AddData(0, -0.4f, 0.15f);
	vb_clickables.AddData(0, -0.3f, 0.1f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	//Felbontás - Jobbnyíl
	vb_clickables.AddData(0, 0, 0.15f);
	vb_clickables.AddData(0, -0.1f, 0.2f);
	vb_clickables.AddData(0, -0.1f, 0.1f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);

	//Fullscreen
	vb_clickables.AddData(0, -0.2f, 0);
	vb_clickables.AddData(0, -0.3f, 0);
	vb_clickables.AddData(0, -0.3f, -0.1f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(0, -0.2f, 0);
	vb_clickables.AddData(0, -0.3f,-0.1f);
	vb_clickables.AddData(0, -0.2f,-0.1f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);

	//Vsync
	vb_clickables.AddData(0, -0.2f, -0.2f);
	vb_clickables.AddData(0, -0.3f, -0.2f);
	vb_clickables.AddData(0, -0.3f, -0.3f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(0, -0.2f, -0.2f);
	vb_clickables.AddData(0, -0.3f, -0.3f);
	vb_clickables.AddData(0, -0.2f, -0.3f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);

	//Music Volume
	vb_clickables.AddData(0, 0.4f, 0.2f);
	vb_clickables.AddData(0, 0.3f, 0.2f);
	vb_clickables.AddData(0, 0.3f, 0.1f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(0, 0.4f, 0.2f);
	vb_clickables.AddData(0, 0.3f, 0.1f);
	vb_clickables.AddData(0, 0.4f, 0.1f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);

	//Sfx volume
	vb_clickables.AddData(0, 0.4f, 0);
	vb_clickables.AddData(0, 0.3f, 0);
	vb_clickables.AddData(0, 0.3f, -0.1f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(0, 0.4f, 0);
	vb_clickables.AddData(0, 0.3f, -0.1f);
	vb_clickables.AddData(0, 0.4f, -0.1f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);

	vb_clickables.InitBuffers();
}

void OptionHandler::SwitchResolution(bool forward)
{
	if (forward) {
		if (displayModeIndex != displayModes.begin())
		{
			--displayModeIndex;
			decoratorTextures.back() = menuRenderer->RenderTextShaded(std::string(std::to_string(displayModeIndex->first) + 'x' + std::to_string(displayModeIndex->second)).c_str());
		}
	}
	else {
		++displayModeIndex;
		if (displayModeIndex != displayModes.end())
		{
			decoratorTextures.back() = menuRenderer->RenderTextShaded(std::string(std::to_string(displayModeIndex->first) + 'x' + std::to_string(displayModeIndex->second)).c_str());
		}
		else --displayModeIndex;
	}
}
