#include "MainMenu.h"

MainMenu::MainMenu(int viewport_w, int viewport_h) : viewportWidth(viewport_w), viewportHeight(viewport_h)
{
	mousePointedData = new float[4];
	mousePointedData[3] = 0.0f;
}

MainMenu::~MainMenu()
{
	delete[] mousePointedData;
	
	delete initialState;
	delete connectState;

	vb_background.Clean();

	vb_fbo.Clean();
	sh_default.Clean();

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

	//glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
	//glEnable(GL_STENCIL_TEST);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	initialState->AddButton(u8"Play");
	initialState->AddButton(u8"Quit");
	initialState->BuildLayout();

	connectState->AddInputBox("IP");
	connectState->AddInputBox("Port");
	connectState->AddButton(u8"Connect");
	connectState->BuildLayout();

	return true;
}

//Ha true akkor csatlakozást jelzünk
bool MainMenu::Update()
{
	//mozgatások/animációk
	static Uint32 lastTime = SDL_GetTicks();
	float deltaTime = (SDL_GetTicks() - lastTime) / 1000.0f;
	//HandleState();
	lastTime = SDL_GetTicks();

	if (connectSignal) {
		return true;
	}
	return false;
}

//void MainMenu::HandleState()
//{
//	
//}

//Rajzolási hívás
void MainMenu::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, dirL_frameBuffer);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	pCurrentState->PreProcess();
	glReadPixels(mouseX, viewportHeight - mouseY - 1, 1, 1, GL_RGBA, GL_FLOAT, (void*)mousePointedData);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sh_default.On();
	sh_default.SetTexture("quadTexture", 0, bgTextures[0]);
	vb_background.On();
	vb_background.Draw(GL_TRIANGLE_STRIP, 0, 4);
	vb_background.Off();
	sh_default.Off();

	pCurrentState->Render();

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
			//std::cout << SDL_GetKeyName(key.keysym.sym) << std::endl;
			pCurrentState->UpdateTexture(typingInInput, SDL_GetKeyName(key.keysym.sym), false);
		}
		else if (SDLK_RETURN == key.keysym.sym) {
			menuState = MenuState::CLICKING;
		}
		else if (SDLK_BACKSPACE == key.keysym.sym) {
			pCurrentState->UpdateTexture(typingInInput, SDL_GetKeyName(key.keysym.sym), true);
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

}

//True-val tér vissza ha be akarjuk zárni a programot magasabb szintrõl
bool MainMenu::MouseDown(SDL_MouseButtonEvent& mouse)
{
	//std::cout << mousePointedData[0] << " g " << mousePointedData[1] << " b " << mousePointedData[2] << " a " << mousePointedData[3] << std::endl;
	if (pCurrentState == initialState) {
		if (mousePointedData[3] == 100) {
			pCurrentState = connectState;
		}
		else if(mousePointedData[3] == 101)
		{
			return true;
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
				connectSignal = true;
				break;
			default:
				break;
			}
		}
	}
	return false;
}

void MainMenu::MouseUp(SDL_MouseButtonEvent& mouse)
{
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