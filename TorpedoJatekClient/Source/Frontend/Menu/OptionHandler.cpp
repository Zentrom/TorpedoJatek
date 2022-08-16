
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
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "[Shader_Link]", "Error during Shader compilation: sh_options", nullptr);
	}
	SDL_DisplayMode mode;
	for (int i = 0; i < SDL_GetNumDisplayModes(0); ++i) {
		if (SDL_GetDisplayMode(0, i, &mode) != 0) {
			std::cout << "[OptionHandler] SDL_GetDisplayMode failed: " << SDL_GetError() << std::endl;
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "[SDL_GetDisplayMode]", SDL_GetError(), nullptr);
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
	fullscreen = (options["Fullscreen"] ? true : false);
	vsync = (options["Vsync"] ? true : false);
	musSliderTransX = 0.4f * options["MusicVolume"] / 128.0f;
	sfxSliderTransX = 0.4f * options["SfxVolume"] / 128.0f;

	//for (std::pair<int, int>& dismode : displayModes) {
	//	std::cout << dismode.first << " x " << dismode.second << std::endl;
	//}
}

OptionHandler::~OptionHandler()
{
	vb_clickables.Clean();
	sh_options.Clean();

	glDeleteTextures(1, &checkBoxOffTexture);
	glDeleteTextures(2, &checkBoxOnTexture);
}

//3D pickinghez elõfeldolgozás,hogy felépítse a clickelhetõ elemek indexét
void OptionHandler::PreProcess()
{
	sh_menu.On();
	sh_menu.SetUniform("is_PreProcess", true);
	int pickingIndex = preProcessOffset;
	vb_button.On();
	for (unsigned int i = 0; i < buttonTextures.size(); ++i) {
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
	sh_menu.SetUniform("translation", musSliderTransX);
	vb_clickables.Draw(GL_TRIANGLES, 18, 6); //musicslider
	sh_menu.SetUniform("clickableIndex", pickingIndex++);
	sh_menu.SetUniform("translation", sfxSliderTransX);
	vb_clickables.Draw(GL_TRIANGLES, 24, 6); //audioslider
	vb_clickables.Off();
	sh_menu.SetUniform("clickableIndex", preProcessOffset * 3);
	sh_menu.SetUniform("translation", 0.0f);
	sh_menu.SetUniform("is_PreProcess", false);
	sh_menu.Off();
}

//Kirajzolás
void OptionHandler::Render(float pointed_element)
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
	vb_button.On();
	for (unsigned int i = 0; i < buttonTextures.size(); ++i) {
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
	sh_options.SetUniform("has_Texture", true);
	if (fullscreen) sh_options.SetTexture("optionTexture", 0, checkBoxOnTexture);
	else sh_options.SetTexture("optionTexture", 0, checkBoxOffTexture);
	sh_options.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 6, 6); //fullscreen
	if (vsync) sh_options.SetTexture("optionTexture", 0, checkBoxOnTexture);
	else sh_options.SetTexture("optionTexture", 0, checkBoxOffTexture);
	sh_options.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 12, 6); //vsync
	sh_options.SetTexture("optionTexture", 0, checkBoxOffTexture);
	sh_options.SetUniform("translation", musSliderTransX);
	sh_options.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 18, 6); //audioslider
	sh_options.SetUniform("translation", sfxSliderTransX);
	sh_options.SetUniform("clickableIndex", pickingIndex++);
	vb_clickables.Draw(GL_TRIANGLES, 24, 6); //audioslider
	sh_options.SetUniform("has_Texture", false);
	sh_options.SetUniform("translation", 0.0f);
	vb_clickables.Off();

	sh_options.Off();
}

//Clickelhetõ elemek hozzáadása a beállítás menühöz
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
	decoratorTextures.push_back(menuRenderer->RenderTextSolid(resolution.c_str()));

	//Felbontás - Balnyíl
	vb_clickables.AddData(0, -0.3f, 0.2f);
	vb_clickables.AddData(0, -0.35f, 0.15f);
	vb_clickables.AddData(0, -0.3f, 0.1f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 1.0f);
	//Felbontás - Jobbnyíl
	vb_clickables.AddData(0, -0.05f, 0.15f);
	vb_clickables.AddData(0, -0.1f, 0.2f);
	vb_clickables.AddData(0, -0.1f, 0.1f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 1.0f);

	//Fullscreen
	vb_clickables.AddData(0, -0.2f, 0);
	vb_clickables.AddData(0, -0.3f, 0);
	vb_clickables.AddData(0, -0.3f, -0.1f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 0);
	vb_clickables.AddData(2, 0, 0);
	vb_clickables.AddData(2, 0, 1.0f);
	vb_clickables.AddData(0, -0.2f, 0);
	vb_clickables.AddData(0, -0.3f,-0.1f);
	vb_clickables.AddData(0, -0.2f,-0.1f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 0);
	vb_clickables.AddData(2, 0, 1.0f);
	vb_clickables.AddData(2, 1.0f, 1.0f);

	//Vsync
	vb_clickables.AddData(0, -0.2f, -0.2f);
	vb_clickables.AddData(0, -0.3f, -0.2f);
	vb_clickables.AddData(0, -0.3f, -0.3f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 0);
	vb_clickables.AddData(2, 0, 0);
	vb_clickables.AddData(2, 0, 1.0f);
	vb_clickables.AddData(0, -0.2f, -0.2f);
	vb_clickables.AddData(0, -0.3f, -0.3f);
	vb_clickables.AddData(0, -0.2f, -0.3f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 0);
	vb_clickables.AddData(2, 0, 1.0f);
	vb_clickables.AddData(2, 1.0f, 1.0f);

	//Music Volume
	vb_clickables.AddData(0, 0.23f, 0.18f);
	vb_clickables.AddData(0, 0.17f, 0.18f);
	vb_clickables.AddData(0, 0.17f, 0.12f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 0);
	vb_clickables.AddData(2, 0, 0);
	vb_clickables.AddData(2, 0, 1.0f);
	vb_clickables.AddData(0, 0.23f, 0.18f);
	vb_clickables.AddData(0, 0.17f, 0.12f);
	vb_clickables.AddData(0, 0.23f, 0.12f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 0);
	vb_clickables.AddData(2, 0, 1.0f);
	vb_clickables.AddData(2, 1.0f, 1.0f);

	//Sfx volume
	vb_clickables.AddData(0, 0.23f, -0.02f);
	vb_clickables.AddData(0, 0.17f, -0.02f);
	vb_clickables.AddData(0, 0.17f, -0.08f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 0);
	vb_clickables.AddData(2, 0, 0);
	vb_clickables.AddData(2, 0, 1.0f);
	vb_clickables.AddData(0, 0.23f, -0.02f);
	vb_clickables.AddData(0, 0.17f, -0.08f);
	vb_clickables.AddData(0, 0.23f, -0.08f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(1, 0.5f, 0.5f, 0.5f, 1.0f);
	vb_clickables.AddData(2, 1.0f, 0);
	vb_clickables.AddData(2, 0, 1.0f);
	vb_clickables.AddData(2, 1.0f, 1.0f);

	vb_clickables.InitBuffers();
}

//Felbontás változtatása
void OptionHandler::SwitchResolution(bool forward)
{
	if (forward) {
		if (displayModeIndex != displayModes.begin())
		{
			--displayModeIndex;
			decoratorTextures.back() = menuRenderer->RenderTextSolid(std::string(std::to_string(displayModeIndex->first) + 'x' + std::to_string(displayModeIndex->second)).c_str());
		}
	}
	else {
		++displayModeIndex;
		if (displayModeIndex != displayModes.end())
		{
			decoratorTextures.back() = menuRenderer->RenderTextSolid(std::string(std::to_string(displayModeIndex->first) + 'x' + std::to_string(displayModeIndex->second)).c_str());
		}
		else --displayModeIndex;
	}
}

//Teljes képernyõ változtatása
void OptionHandler::SwitchFullscreen()
{
	fullscreen = !fullscreen;
}

//Vsync változtatása
void OptionHandler::SwitchVsync()
{
	vsync = !vsync;
}

//Csúszkamozgatás backend lekezelése
void OptionHandler::HandleSlider(float pointed, Sint32 mouse_x)
{
	int width = optionsRef["ResolutionWidth"];
	float startPos = (width / 20.0f) * 12.0f;
	float endPos = (width / 20.0f) * 16.0f;
	float calcValue = (static_cast<float>(mouse_x) - startPos) / (width / 2.0f);

	if (calcValue > 0 && mouse_x < endPos) {
		if (pointed == 106) musSliderTransX = calcValue;
		else if (pointed == 107) sfxSliderTransX = calcValue;
	}
	else if (calcValue < 0) {
		if (pointed == 106) musSliderTransX = 0.0f;
		else if (pointed == 107) sfxSliderTransX = 0.0f;
	}
	else if (mouse_x > endPos) {
		if (pointed == 106) musSliderTransX = 0.4f;
		else if (pointed == 107) sfxSliderTransX = 0.4f;
	}
}

//Elmozgatott csúszkából kiszámítja az új hangerõ értéket
void OptionHandler::UpdateVolume()
{
	Mix_VolumeMusic(static_cast<int>((musSliderTransX * 128) / 0.4f));
	Mix_Volume(-1, static_cast<int>((sfxSliderTransX * 128) / 0.4f));
}

//Beállítások érvényesítése és fájlba mentése
void OptionHandler::ApplySettings(SDL_Window* window)
{
	if (!fullscreen) {
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowSize(window, displayModeIndex->first, displayModeIndex->second);
	}
	else {
		SDL_DisplayMode target = {0, displayModeIndex->first, displayModeIndex->second, 0, 0};
		SDL_DisplayMode closest;
		SDL_GetClosestDisplayMode(0, &target, &closest);
		//std::cout << SDL_BITSPERPIXEL(closest.format) << "bpp " << closest.w << 'x' << closest.h << ' ' << closest.refresh_rate << "Hz" << std::endl;

		SDL_SetWindowDisplayMode(window, &closest);
		SDL_SetWindowSize(window, closest.w, closest.h); //EZ BUGOS AZ SDL-be|Megkell hívni most Fullscreennél is
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}

	SDL_GL_SetSwapInterval(vsync);

	optionsRef["ResolutionWidth"] = displayModeIndex->first;
	optionsRef["ResolutionHeight"] = displayModeIndex->second;
	optionsRef["Fullscreen"] = fullscreen;
	optionsRef["Vsync"] = vsync;
	optionsRef["MusicVolume"] = Mix_VolumeMusic(-1);
	optionsRef["SfxVolume"] = Mix_Volume(1, -1);

	std::fstream optionsFile("Resources/options.cfg", std::ios::out);
	for (std::map<std::string, int>::const_iterator iter = optionsRef.cbegin(); iter != optionsRef.cend(); ++iter) {
	optionsFile << iter->first << '=' << iter->second << '\n';
	}
	optionsFile.close();
}

//Beállítások elvetése
void OptionHandler::CancelSettings()
{
	displayModeIndex = std::find(displayModes.begin(), displayModes.end(), std::pair<int, int>(optionsRef["ResolutionWidth"], optionsRef["ResolutionHeight"]));
	decoratorTextures.back() = menuRenderer->RenderTextSolid(std::string(std::to_string(displayModeIndex->first) + 'x' + std::to_string(displayModeIndex->second)).c_str());

	fullscreen = (optionsRef["Fullscreen"] ? true : false);
	vsync = (optionsRef["Vsync"] ? true : false);
	musSliderTransX = 0.4f * optionsRef["MusicVolume"] / 128.0f;
	sfxSliderTransX = 0.4f * optionsRef["SfxVolume"] / 128.0f;
	UpdateVolume();
}
