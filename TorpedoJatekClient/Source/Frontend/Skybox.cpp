
#include "Skybox.h"

Skybox::Skybox(void)
{
}

Skybox::~Skybox(void)
{
	glDeleteTextures(1, &cubeTextureID);
}

//Inicializálja a skyboxhoz szükséges dolgokat
void Skybox::Init()
{
	CreateCubeMap();

	vb_skybox.AddAttribute(0, 3);

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
			std::cout << "Couldn't load skybox face from path: " << facePaths[i] << '\n';
			std::cout << IMG_GetError() << std::endl;
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
	//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

//Kirajzolja a skyboxot
void Skybox::Draw(gCamera & camera, gShaderProgram & sh_program)
{
	glDepthFunc(GL_LEQUAL);

	glm::mat4 viewProj = camera.GetProj() * glm::mat4(glm::mat3(camera.GetViewMatrix()));
	sh_program.SetUniform("viewProj", viewProj);
	vb_skybox.On();
	vb_skybox.DrawIndexed(GL_TRIANGLES, 0, 36, 0);
	vb_skybox.Off();

	glDepthFunc(GL_LESS);
}
