#pragma once

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <SDL_image.h>

#include <array>
#include <iostream>

//A pályát körülvevõ skybox
class Skybox 
{
public:
	Skybox(void);
	~Skybox(void);

	void Init();
	void Draw(gCamera &camera, gShaderProgram &sh_program);
private:
	void CreateCubeMap();
	std::array<const std::string, 6> facePaths = { //A skybox oldalainak az elérési útvonalai
		//"Resources/Textures/Skybox/right.jpg", "Resources/Textures/Skybox/left.jpg",
		//"Resources/Textures/Skybox/top.jpg", "Resources/Textures/Skybox/bottom.jpg",
		//"Resources/Textures/Skybox/back.jpg", "Resources/Textures/Skybox/front.jpg"

		"Resources/Textures/Skybox/xpos.png", "Resources/Textures/Skybox/xneg.png",
		"Resources/Textures/Skybox/ypos.png", "Resources/Textures/Skybox/yneg.png",
		"Resources/Textures/Skybox/zpos.png", "Resources/Textures/Skybox/zneg.png"
	};

	GLuint cubeTextureID;
	gVertexBuffer	vb_skybox; //grafikai adatok
};