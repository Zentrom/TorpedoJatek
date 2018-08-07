#pragma once

//#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../Utils/gVertexBuffer.h"
#include "../../Utils/gShaderProgram.h"
#include "../../Utils/gCamera.h"

#include "../Globals.h"

class PlayTile
{
public:
	PlayTile(void);
	PlayTile(glm::vec3 translate);
	~PlayTile(void);

	void Init();
	void Draw(gCamera &m_camera,gShaderProgram &sh_playtile);
	void setState(int newState);

protected:
	gVertexBuffer	vb_playtile;
	
	glm::vec3 tileState = glm::vec3(0, 0, 1); //3kek,2sarga,1piros

	glm::vec3 playtile_translate = glm::vec3(0, 0, 0)*GLOBALScale;
	float playtile_rotate = 0.0f;
	glm::vec3 playtile_rotate_angle = glm::vec3(0, 1.0f, 0);
	glm::vec3 playtile_scale = glm::vec3(2.0f, 1.0f, 2.0f)*GLOBALScale;

};
