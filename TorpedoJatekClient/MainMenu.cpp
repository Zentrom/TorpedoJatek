#include "MainMenu.h"

MainMenu::MainMenu(const TorpedoVersion& version, std::map<std::string, int>& options) : viewportWidth(options["ResolutionWidth"]), viewportHeight(options["ResolutionHeight"])
{
	versionString = std::string('v' + std::to_string(version.majorVersion) + '.' + std::to_string(version.betaVersion) + '.'
		+ std::to_string(version.alphaVersion) + version.experimentalVersion);
	versionString.append(u8" made by Negrut Ákos");

	Mix_VolumeMusic(options["MusicVolume"]);
	Mix_Volume(-1, options["SfxVolume"]);
	optionsState = new OptionHandler(options);

	mousePointedData = new float[4];
	mousePointedData[3] = 0.0f;
}

MainMenu::~MainMenu()
{
	delete[] mousePointedData;
	
	delete initialState;
	delete connectState;
	delete optionsState;

	vb_background.Clean();

	vb_fbo.Clean();
	sh_default.Clean();

	Mix_FreeMusic(menuMusic);
	Mix_FreeChunk(clickSound);

	glDeleteTextures(1, &logoTexture);
	glDeleteTextures(1, &elementsBg);
	glDeleteTextures(1, &menuRail);
	for (GLuint& texture : bgTextures) {
		glDeleteTextures(1, &texture);
	}
	if (dirL_frameBufferCreated)
	{
		//glDeleteRenderbuffers(1, &dirL_depthStencilBuffer);
		glDeleteTextures(1, &dirL_colorBuffer);
		glDeleteFramebuffers(1, &dirL_frameBuffer);
	}
}

//Menü inicializálása
bool MainMenu::Init()
{
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	CreateFrameBuffer(viewportWidth, viewportHeight);
	vb_fbo.AddAttribute(0, 2); //position
	vb_fbo.AddAttribute(1, 2); //textcoord
	
	vb_fbo.AddData(0, -1, -1);
	vb_fbo.AddData(0, 1, -1);
	vb_fbo.AddData(0, -1, 1);
	vb_fbo.AddData(0, 1, 1);

	vb_fbo.AddData(1, 0, 0);
	vb_fbo.AddData(1, 1, 0);
	vb_fbo.AddData(1, 0, 1);
	vb_fbo.AddData(1, 1, 1);
	vb_fbo.InitBuffers();

	vb_background.AddAttribute(0, 2); //position
	vb_background.AddAttribute(1, 2); //textcoord

	vb_background.AddData(0, -1, -1);
	vb_background.AddData(0, 1, -1);
	vb_background.AddData(0, -1, 1);
	vb_background.AddData(0, 1, 1);

	vb_background.AddData(1, 0, 1);
	vb_background.AddData(1, 1, 1);
	vb_background.AddData(1, 0, 0);
	vb_background.AddData(1, 1, 0);
	vb_background.InitBuffers();

	sh_default.AttachShader(GL_VERTEX_SHADER, "Shaders/default.vert");
	sh_default.AttachShader(GL_FRAGMENT_SHADER, "Shaders/default.frag");
	sh_default.BindAttribLoc(0, "vs_in_pos");
	sh_default.BindAttribLoc(1, "vs_in_tex");
	if (!sh_default.LinkProgram()) {
		std::cout << "[Shader_Link]Error during Shader compilation: sh_default" << std::endl;
		return false;
	}

	initialState->AddDecoratorTexture(0.0f, 0.7f, 0.6f, 0.2f, logoTexture);
	initialState->AddDecoratorString(0.5f, -0.8f, 0.4f, versionString.c_str());
	initialState->AddButton(0, 0, u8"Play");
	initialState->AddButton(0, 0, u8"Debug");
	initialState->AddButton(0, 0, u8"Options");
	initialState->AddButton(0, 0, u8"Quit");
	initialState->BuildLayout();

	connectState->AddDecoratorTexture(0.0f, 0.1f, 0.6f, 0.4f, elementsBg);
	connectState->AddDecoratorString(-0.4f, 0.4f, 0.1f, u8"IP:");
	connectState->AddDecoratorString(-0.4f, 0.2f, 0.1f, u8"Port:");
	connectState->AddInputBox(nullptr); //ip
	connectState->AddInputBox(nullptr); //port
	connectState->AddButton(0.3f, 0.01f, u8"Connect");
	connectState->AddButton(-0.3f, 0.01f, u8"Back");
	connectState->BuildLayout();

	optionsState->AddDecoratorTexture(0.0f, 0.0f, 0.7f, 0.6f, elementsBg);
	optionsState->AddDecoratorString(-0.3f, 0.4f, 0.2f, u8"Video", 0.1f);
	optionsState->AddDecoratorString(-0.5f, 0.2f, 0.1f, u8"Resolution:");
	optionsState->AddDecoratorString(-0.5f, 0.0f, 0.1f, u8"Fullscreen:");
	optionsState->AddDecoratorString(-0.5f,-0.2f, 0.1f, u8"Vsync:");
	optionsState->AddDecoratorString(0.3f, 0.4f, 0.2f, u8"Audio", 0.1f);
	optionsState->AddDecoratorString(0.1f, 0.2f, 0.08f, u8"Music:");
	optionsState->AddDecoratorString(0.1f, 0.0f, 0.08f, u8"Sfx:");
	optionsState->AddDecoratorTexture(0.0f, 0.0f, 0.01f, 0.5f, menuRail);
	optionsState->AddDecoratorTexture(0.4f, 0.15f, 0.2f, 0.01f, menuRail);
	optionsState->AddDecoratorTexture(0.4f,-0.05f, 0.2f, 0.01f, menuRail);
	optionsState->AddClickableOptions();
	optionsState->AddButton(-0.4f, -0.4f, u8"Back");
	optionsState->AddButton(0.4f, -0.4f, u8"Apply");
	optionsState->BuildLayout();

	menuMusic = Mix_LoadMUS("Resources/Audio/menuMusic.ogg");
	if (!menuMusic) {
		printf("Mix_LoadMUS(\"menuMusic.ogg\"): %s\n", Mix_GetError());
	}
	clickSound = Mix_LoadWAV("Resources/Audio/menuClick.wav");
	if (!clickSound) {
		printf("Mix_LoadWAV error: %s\n", SDL_GetError());
	}
	Mix_PlayMusic(menuMusic, 10);

	return true;
}

//Ha true akkor csatlakozást jelzünk
void MainMenu::Update()
{
	static Uint32 lastTime = SDL_GetTicks();
	float deltaTime = (SDL_GetTicks() - lastTime) / 1000.0f;
	lastTime = SDL_GetTicks();

	if (menuState == MenuState::TYPING) {
		typingAnimElapsed += deltaTime;
		if (typingAnimElapsed > typingAnimTime) {
			cursorShown = !cursorShown;
			typingAnimElapsed = 0.0f;
			pCurrentState->UpdateInputBox(typingInInput, nullptr, false, cursorShown);
		}
	}

	bgAnimElapsed += deltaTime;
	if (bgAnimElapsed > bgAnimTime)
	{
		if (++bgIndex == bgTextures.size()) {
			bgIndex = 0;
		}
		bgAnimElapsed = 0.0f;
	}
}

//Rajzolási hívás
void MainMenu::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, dirL_frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (pCurrentState == optionsState) {
		optionsState->PreProcess();
	}
	else {
		pCurrentState->PreProcess();
	}
	glReadPixels(mouseX, viewportHeight - mouseY - 1, 1, 1, GL_RGBA, GL_FLOAT, (void*)mousePointedData);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sh_default.On();
	sh_default.SetTexture("quadTexture", 0, bgTextures[bgIndex]);
	vb_background.On();
	vb_background.Draw(GL_TRIANGLE_STRIP, 0, 4);
	vb_background.Off();
	sh_default.Off();

	if (menuState == MenuState::TYPING) {
		pCurrentState->Render(66.0f);
	}
	else {
		if (pCurrentState == optionsState) {
			optionsState->Render(mousePointedData[3]);
		}
		else {
			pCurrentState->Render(mousePointedData[3]);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sh_default.On();
	sh_default.SetTexture("quadTexture", 0, dirL_colorBuffer);
	vb_fbo.On();
	vb_fbo.Draw(GL_TRIANGLE_STRIP, 0, 4);
	vb_fbo.Off();
	sh_default.Off();
}

void MainMenu::KeyboardDown(SDL_KeyboardEvent& key)
{
	if (menuState == MenuState::TYPING) {
		if ((key.keysym.sym >= SDLK_0 && key.keysym.sym <= SDLK_9)
			|| (key.keysym.sym >= SDLK_a && key.keysym.sym <= SDLK_z)
			|| key.keysym.sym == SDLK_PERIOD) 
		{
			pCurrentState->UpdateInputBox(typingInInput, SDL_GetKeyName(key.keysym.sym), false, cursorShown);
		}
		else if (SDLK_RETURN == key.keysym.sym || SDLK_ESCAPE == key.keysym.sym) {
			menuState = MenuState::CLICKING;
			typingAnimElapsed = 0.0f;
			cursorShown = true;
			pCurrentState->UpdateInputBox(typingInInput, nullptr, false, false);
		}
		else if (SDLK_BACKSPACE == key.keysym.sym) {
			pCurrentState->UpdateInputBox(typingInInput, SDL_GetKeyName(key.keysym.sym), true, cursorShown);
		}
	}
}

void MainMenu::KeyboardUp(SDL_KeyboardEvent& key)
{
	
}

void MainMenu::MouseMove(SDL_MouseMotionEvent& mouse)
{
	mouseX = mouse.x;
	mouseY = mouse.y;

	if (pCurrentState == optionsState) {
		if (mouse.state == SDL_BUTTON_LMASK 
			&& (mousePointedData[3] == 106 || mousePointedData[3] == 107)) {
			optionsState->HandleSlider(mousePointedData[3], mouseX);
		}
	}
}

//True-val tér vissza ha be akarjuk zárni a programot magasabb szintrõl
MenuSignal MainMenu::MouseDown(SDL_MouseButtonEvent& mouse, SDL_Window* window)
{
	//std::cout << mouseX << " " << mouseY << std::endl;

	if (pCurrentState == initialState) {
		if (mousePointedData[3] == 100) {
			//Play
			pCurrentState = connectState;
		}
		else if (mousePointedData[3] == 101) {
			//Debug
			return MenuSignal::DEBUG;
		}
		else if (mousePointedData[3] == 102) {
			//Options
			pCurrentState = optionsState;
		}
		else if(mousePointedData[3] == 103) {
			//Quit
			return MenuSignal::QUIT;
		}
	}
	else if (pCurrentState == connectState) {
		if (menuState == MenuState::CLICKING) {
			std::vector<std::string> ipAndPort;
			switch (static_cast<int>(mousePointedData[3]))
			{
			case 100:
				//ip input
				menuState = MenuState::TYPING;
				typingInInput = 100;
				break;
			case 101:
				//port input
				menuState = MenuState::TYPING;
				typingInInput = 101;
				break;
			case 102:
				//connect gomb
				ipAndPort = connectState->getInputStrings(2);
				connectIP = ipAndPort.at(0);
				connectPort = ipAndPort.at(1);
				return MenuSignal::CONNECT;
			case 103:
				//Back gomb
				//connectState->Clean();
				pCurrentState = initialState;
				break;
			default:
				break;
			}
		}
	}
	else if (pCurrentState == optionsState) {
		switch (static_cast<int>(mousePointedData[3]))
		{
		case 100:
			//Back gomb
			optionsState->CancelSettings();
			pCurrentState = initialState;
			break;
		case 101:
			//Apply gomb
			optionsState->ApplySettings(window);
			break;
		case 102:
			//Bal nyíl
			optionsState->SwitchResolution(false);
			break;
		case 103:
			//Jobb nyíl
			optionsState->SwitchResolution(true);
			break;
		case 104:
			//Fullscreen
			optionsState->SwitchFullscreen();
			break;
		case 105:
			//Vsync
			optionsState->SwitchVsync();
			break;
		default:
			break;
		}
	}
	return MenuSignal::CONTINUE;
}

void MainMenu::MouseUp(SDL_MouseButtonEvent& mouse)
{
	Mix_PlayChannel(1, clickSound, 0);

	if (pCurrentState == optionsState) {
		optionsState->UpdateVolume();
	}
}

void MainMenu::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

//Ablak átméretezéskor hívódik
void MainMenu::Resize(int width, int height)
{
	glViewport(0, 0, width, height);

	viewportWidth = width;
	viewportHeight = height;

	CreateFrameBuffer(width, height);
}

//Custom Framebuffer létrehozása
void MainMenu::CreateFrameBuffer(int width, int height)
{
	if (dirL_frameBufferCreated)
	{
		//glDeleteRenderbuffers(1, &dirL_depthStencilBuffer);
		glDeleteTextures(1, &dirL_colorBuffer);
		glDeleteFramebuffers(1, &dirL_frameBuffer);
	}

	glGenFramebuffers(1, &dirL_frameBuffer);
	glGenTextures(1, &dirL_colorBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, dirL_frameBuffer);
	glBindTexture(GL_TEXTURE_2D, dirL_colorBuffer);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dirL_colorBuffer, 0);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cout << "[Create_FBO]Error creating color attachment" << std::endl;
		char ch; std::cin >> ch;
		exit(1);
	}

	//glGenRenderbuffers(1, &dirL_depthStencilBuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, dirL_depthStencilBuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dirL_depthBuffer);
	//if (glGetError() != GL_NO_ERROR)
	//{
	//	std::cout << "[Create_FBO]Error creating depth/stencil attachment" << std::endl;
	//	char ch;
	//	std::cin >> ch;
	//	exit(1);
	//}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[Create_FBO]Incomplete framebuffer (";
		switch (status) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "GL_FRAMEBUFFER_UNSUPPORTED";
			break;
		}
		std::cout << ")" << std::endl;
		char ch;
		std::cin >> ch;
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	dirL_frameBufferCreated = true;
}

std::string MainMenu::getIP()
{
	return connectIP;
}

std::string MainMenu::getPort()
{
	return connectPort;
}