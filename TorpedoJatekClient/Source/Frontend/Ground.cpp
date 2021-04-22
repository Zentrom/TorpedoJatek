
#include "Ground.h"

const float Ground::scaleXZ = SeaTile::getScaleXZ() * 2.0f;

Ground::Ground(void)
{

}

Ground::Ground(const glm::vec3 &terrainTranslate)
{
	ground_translate += terrainTranslate;
}

Ground::~Ground(void)
{
}

//Inicializálja a földdarab grafikai modelljét
void Ground::Init()
{
	vb_ground.AddAttribute(0, 3);
	vb_ground.AddAttribute(1, 4);
	vb_ground.AddAttribute(2, 3);
	vb_ground.AddAttribute(3, 2);

	vb_ground.AddData(0, -0.5f, 0, -0.5f);
	vb_ground.AddData(0, 0.5f, 0, -0.5f);
	vb_ground.AddData(0, -0.5f, 0, 0.5f);
	vb_ground.AddData(0, 0.5f, 0, 0.5f);

	vb_ground.AddData(1, 0.4f, 0.4f, 0, 1);
	vb_ground.AddData(1, 0.4f, 0.4f, 0, 1);
	vb_ground.AddData(1, 0.4f, 0.4f, 0, 1);
	vb_ground.AddData(1, 0.4f, 0.4f, 0, 1);

	vb_ground.AddData(2, 0, 1, 0);
	vb_ground.AddData(2, 0, 1, 0);
	vb_ground.AddData(2, 0, 1, 0);
	vb_ground.AddData(2, 0, 1, 0);

	vb_ground.AddData(3, 0, 0);
	vb_ground.AddData(3, 1, 0);
	vb_ground.AddData(3, 0, 1);
	vb_ground.AddData(3, 1, 1);

	vb_ground.AddIndex(1, 0, 2);
	vb_ground.AddIndex(1, 2, 3);

	vb_ground.InitBuffers();

	
}

//Kirajzol egy földdarabot
void Ground::Draw(gCamera &camera, gShaderProgram &sh_program, GLuint &textureID)
{

	glm::mat4 matWorld = glm::translate(ground_translate) * glm::rotate(ground_rotate, ground_rotate_angle) * glm::scale(ground_scale);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() * matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	sh_program.SetUniform("eye_pos", camera.GetEye());

	sh_program.SetUniform("hasTexture", true);
	sh_program.SetTexture("texImage", 0, textureID);

	vb_ground.On();
	vb_ground.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	vb_ground.Off();

	sh_program.SetUniform("hasTexture", false);
}

float Ground::getScaleXZ()
{
	return scaleXZ;
}
