
#include "Ground.h"

Ground::Ground(void)
{

}

Ground::Ground(glm::vec3 terrainTranslate)
{
	ground_translate = ground_translate + terrainTranslate;
}

Ground::~Ground(void)
{
}

void Ground::Init()
{
	vb_ground.AddAttribute(0, 3);
	vb_ground.AddAttribute(1, 3);
	
	vb_ground.AddData(0, -0.5f, 0, -0.5f);
	vb_ground.AddData(0, 0.5f, 0, -0.5f);
	vb_ground.AddData(0, -0.5f, 0, 0.5f);
	vb_ground.AddData(0, 0.5f, 0, 0.5f);
	
	vb_ground.AddData(1, 0.6f, 0.6f, 0);
	vb_ground.AddData(1, 0.6f, 0.6f, 0);
	vb_ground.AddData(1, 0.6f, 0.6f, 0);
	vb_ground.AddData(1, 0.6f, 0.6f, 0);
	
	vb_ground.AddIndex(1, 0, 2);
	vb_ground.AddIndex(1, 2, 3);
	
	vb_ground.InitBuffers();
}

void Ground::Draw(gCamera &camera, gShaderProgram &sh_program)
{

	glm::mat4 matWorld = glm::translate(ground_translate) * glm::rotate(ground_rotate, ground_rotate_angle) * glm::scale(ground_scale);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	//sh_program.SetUniform("world", matWorld);
	//sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);

	//m_program.SetTexture("texImage", 0, m_groundTextureID);
	//m_program.SetTexture("texNormal", 1, m_groundNormalMapID);

	vb_ground.On();
	vb_ground.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	vb_ground.Off();
}