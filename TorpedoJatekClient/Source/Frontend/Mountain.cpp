
#include "Mountain.h"

const float Mountain::heightY = 6.0f;
const float Mountain::widthX = 6.0f;

Mountain::Mountain(void)
{

}

Mountain::~Mountain(void)
{
}

//Inicializálja a hegy grafikai modelljét
void Mountain::Init()
{
	vb_mountain.AddAttribute(0, 3);
	vb_mountain.AddAttribute(1, 4);
	vb_mountain.AddAttribute(2, 3);

	vb_mountain.AddData(0, -0.5f, -0.5f, 0.5f);
	vb_mountain.AddData(0, 0.5f, -0.5f, 0.5f);
	vb_mountain.AddData(0, -0.5f, 0.5f, 0.5f);
	vb_mountain.AddData(0, 0.5f, 0.5f, 0.5f);
	vb_mountain.AddData(0, -0.5f, -0.5f, -0.5f);
	vb_mountain.AddData(0, 0.5f, -0.5f, -0.5f);
	vb_mountain.AddData(0, -0.5f, 0.5f, -0.5f);
	vb_mountain.AddData(0, 0.5f, 0.5f, -0.5f);

	vb_mountain.AddData(1, 1.0f, 1.0f, 0, 1);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0, 1);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0, 1);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0, 1);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0, 1);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0, 1);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0, 1);
	vb_mountain.AddData(1, 1.0f, 1.0f, 0, 1);

	vb_mountain.AddData(2, -0.5f, -0.5f, 0.5f);
	vb_mountain.AddData(2, 0.5f, -0.5f, 0.5f);
	vb_mountain.AddData(2, -0.5f, 0.5f, 0.5f);
	vb_mountain.AddData(2, 0.5f, 0.5f, 0.5f);
	vb_mountain.AddData(2, -0.5f, -0.5f, -0.5f);
	vb_mountain.AddData(2, 0.5f, -0.5f, -0.5f);
	vb_mountain.AddData(2, -0.5f, 0.5f, -0.5f);
	vb_mountain.AddData(2, 0.5f, 0.5f, -0.5f);

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

//Kirajzolja a hegyet
void Mountain::Draw(gCamera &camera, gShaderProgram &sh_program)
{
	glm::mat4 matWorld = glm::translate(mountain_translate) * glm::rotate(mountain_rotate, mountain_rotate_angle) * glm::scale(mountain_scale);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	sh_program.SetUniform("eye_pos", camera.GetEye());

	//m_program.SetTexture("texImage", 0, m_groundTextureID);
	//m_program.SetTexture("texNormal", 1, m_groundNormalMapID);

	vb_mountain.On();
	vb_mountain.DrawIndexed(GL_TRIANGLES, 0, 36, 0);
	vb_mountain.Off();
}

//Visszaadja hegy szélességét
float Mountain::getWidth()
{
	return widthX;
}

float Mountain::getHeight()
{
	return heightY;
}
