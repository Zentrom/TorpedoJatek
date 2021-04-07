
#include "Mountain.h"

Mountain::Mountain(void)
{

}

Mountain::~Mountain(void)
{
}

void Mountain::Init()
{
	vb_mountain.AddAttribute(0, 3);
	vb_mountain.AddAttribute(1, 3);
	   
	vb_mountain.AddData(0, -0.5f, -0.5f, 0.5f);
	vb_mountain.AddData(0, 0.5f, -0.5f, 0.5f);
	vb_mountain.AddData(0, -0.5f, 0.5f, 0.5f);
	vb_mountain.AddData(0, 0.5f, 0.5f, 0.5f);
	vb_mountain.AddData(0, -0.5f, -0.5f, -0.5f);
	vb_mountain.AddData(0, 0.5f, -0.5f, -0.5f);
	vb_mountain.AddData(0, -0.5f, 0.5f, -0.5f);
	vb_mountain.AddData(0, 0.5f, 0.5f, -0.5f);
	   
	vb_mountain.AddData(1, 1.0f, 1.0f, 0);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0);
	   
	vb_mountain.AddIndex(1, 2, 0);
	vb_mountain.AddIndex(1, 3, 2);
	vb_mountain.AddIndex(5, 3, 1);
	vb_mountain.AddIndex(5, 7, 3);
	vb_mountain.AddIndex(4, 7, 5);
	vb_mountain.AddIndex(4, 6, 7);
	vb_mountain.AddIndex(0, 6, 4);
	vb_mountain.AddIndex(0, 2, 6);
	vb_mountain.AddIndex(3, 6, 2);
	vb_mountain.AddIndex(3, 7, 6);
	vb_mountain.AddIndex(5, 0, 4);
	vb_mountain.AddIndex(5, 1, 0);
	   
	vb_mountain.InitBuffers();
}

void Mountain::Draw(gCamera &camera,gShaderProgram &sh_program) 
{

	glm::mat4 matWorld = glm::translate(mountain_translate) * glm::rotate(mountain_rotate,mountain_rotate_angle) * glm::scale(mountain_scale);
	//glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	//sh_program.SetUniform("world", matWorld);
	//sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);

	//m_program.SetTexture("texImage", 0, m_groundTextureID);
	//m_program.SetTexture("texNormal", 1, m_groundNormalMapID);

	vb_mountain.On();
	vb_mountain.DrawIndexed(GL_TRIANGLES, 0, 36, 0);
	vb_mountain.Off();
}