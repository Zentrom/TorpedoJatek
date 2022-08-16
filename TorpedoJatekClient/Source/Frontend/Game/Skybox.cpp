
#include "Skybox.h"

Skybox::Skybox()
{
}

Skybox::~Skybox()
{
	vb_skybox.Clean();
	sh_skybox.Clean();
	glDeleteTextures(1, &cubeTextureID);
}

//Inicializálja a skyboxhoz szükséges dolgokat
void Skybox::Init()
{
	CreateCubeMap();

	vb_skybox.AddAttribute(0, 3); //pos

	vb_skybox.AddData(0, -1.0f, -1.0f,  1.0f);
	vb_skybox.AddData(0,  1.0f, -1.0f,  1.0f);
	vb_skybox.AddData(0, -1.0f,  1.0f,  1.0f);
	vb_skybox.AddData(0,  1.0f,  1.0f,  1.0f);
	vb_skybox.AddData(0, -1.0f, -1.0f, -1.0f);
	vb_skybox.AddData(0,  1.0f, -1.0f, -1.0f);
	vb_skybox.AddData(0, -1.0f,  1.0f, -1.0f);
	vb_skybox.AddData(0,  1.0f,  1.0f, -1.0f);

	//X
	vb_skybox.AddIndex(3, 7, 5);
	vb_skybox.AddIndex(3, 5, 1);
	vb_skybox.AddIndex(6, 2, 0);
	vb_skybox.AddIndex(6, 0, 4);
	//Y
	vb_skybox.AddIndex(3, 2, 6);
	vb_skybox.AddIndex(3, 6, 7);
	vb_skybox.AddIndex(4, 1, 5);
	vb_skybox.AddIndex(4, 0, 1);
	//Z
	vb_skybox.AddIndex(7, 6, 4);
	vb_skybox.AddIndex(7, 4, 5);
	vb_skybox.AddIndex(2, 3, 0);
	vb_skybox.AddIndex(3, 1, 0);

	vb_skybox.InitBuffers();

	sh_skybox.AttachShader(GL_VERTEX_SHADER, "Shaders/skybox.vert");
	sh_skybox.AttachShader(GL_FRAGMENT_SHADER, "Shaders/skybox.frag");
	sh_skybox.BindAttribLoc(0, "vs_in_pos");
	if (!sh_skybox.LinkProgram()) {
		std::cout << "[Shader_Link]Error during Shader compilation: sh_skybox" << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "[Shader_Link]", "Error during Shader compilation: sh_skybox", nullptr);
	}
	else {
		sh_skybox.On();
		sh_skybox.SetCubeTexture("skyboxCube", 0, cubeTextureID);
		sh_skybox.Off();
	}
}

//Betölti és létrehozza a skybox oldalait
void Skybox::CreateCubeMap()
{
	glGenTextures(1, &cubeTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);

	unsigned int img_mode;
	for (unsigned int i = 0; i < facePaths.size(); i++) {
		SDL_Surface* loaded_img = IMG_Load(facePaths[i].c_str());

		if (!loaded_img) {
			std::cout << "[IMG_Load]Couldn't load skybox face from path: " << facePaths[i] << '\n';
			std::cout << IMG_GetError() << std::endl;
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "[Skybox_CreateCubeMap]", IMG_GetError(), nullptr);
			continue;
		}
		if (loaded_img->format->BytesPerPixel == 4) {
			img_mode = GL_RGBA;
		}
		else {
			img_mode = GL_RGB;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, img_mode, loaded_img->w, loaded_img->h, 0,
			img_mode, GL_UNSIGNED_BYTE, loaded_img->pixels);

		SDL_FreeSurface(loaded_img);
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

//Kirajzolja a skyboxot
void Skybox::Draw(const gCamera& camera)
{
	glDepthFunc(GL_LEQUAL);
	glm::mat4 viewProj = camera.GetProj() * glm::mat4(glm::mat3(camera.GetViewMatrix()));

	sh_skybox.On();
	sh_skybox.SetUniform("viewProj", viewProj);

	vb_skybox.On();
	vb_skybox.DrawIndexed(GL_TRIANGLES, 0, 36);
	vb_skybox.Off();
	sh_skybox.Off();

	glDepthFunc(GL_LESS);
}
