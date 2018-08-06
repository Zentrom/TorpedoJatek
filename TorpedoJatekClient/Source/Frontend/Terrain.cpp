
#include "Terrain.h"

#include "../../Utils/gCamera.h"
#include "../../Utils/gShaderProgram.h"


Terrain::Terrain(void)
{
	float ground_transX = 0;
	float ground_transZ = 0;
	float ground_mountainY = -3.01f;
	glm::vec3 groundResult = glm::vec3(0.0f);
	for (int i = 0; i < (7 * 7); i++) {
		ground_transX = (i % 7) * 2.0f;
		ground_transZ = ((i / 7) * 2.0f) - (3.0f*2.0f);

		groundResult += glm::vec3(ground_transX, ground_mountainY, ground_transZ);

		myGrounds[i] = Ground(groundResult);
		

		//enemyPlayTiles[i].Init();
		//myPlayTiles[i].Init();
		groundResult = glm::vec3(0.0f);
	}
}

Terrain::~Terrain(void)
{
}

void Terrain::Init()
{
	for (int i = 0; i < 49; i++) {
		myGrounds[i].Init();
	}
}

void Terrain::Draw(gCamera &m_camera, gShaderProgram &m_program)
{

	//glm::mat4 matWorld = glm::translate(mountain_translate) * glm::rotate(mountain_rotate, mountain_rotate_angle) * glm::scale(mountain_scale);
	//glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	//glm::mat4 mvp = m_camera.GetViewProj() *matWorld;

	//m_program.SetUniform("world", matWorld);
	//m_program.SetUniform("worldIT", matWorldIT);
	//m_program.SetUniform("MVP", mvp);
	//vb_mountain.On();
	//vb_mountain.DrawIndexed(GL_TRIANGLES, 0, 36, 0);
	//vb_mountain.Off();
	
	for (int i = 0; i < 49; i++) {
		myGrounds[i].Draw(m_camera,m_program);
	}
	
}