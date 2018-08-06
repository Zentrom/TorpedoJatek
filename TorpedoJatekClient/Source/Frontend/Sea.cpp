#include "Sea.h"

#include "../../Utils/gCamera.h"
#include "../../Utils/gShaderProgram.h"

#include <iostream>
Sea::Sea(void)
{

}

Sea::~Sea(void)
{
}

void Sea::Init()
{
	float tile_transX = 0;
	float tile_transZ = 0;
	glm::vec3 tileResult = glm::vec3(0.0f);
	for (int i = 0; i < (7 * 7); i++) {
		tile_transX = (i % 7) * 2.0f;
		tile_transZ = ((i / 7) * 2.0f) - (3.0f*2.0f);

		tileResult += glm::vec3(mountaincenter_border_Xoffset, 0, 0) +
			glm::vec3(mountain_tile_offset, 0, 0) +
			glm::vec3(tile_transX, 0, tile_transZ);

		enemyPlayTiles[i] = PlayTile(tileResult);
		myPlayTiles[i] = PlayTile(glm::vec3(-1, 1, 1)*tileResult);

		enemyPlayTiles[i].Init();
		myPlayTiles[i].Init();
		tileResult = glm::vec3(0.0f);
	}
}

void Sea::Draw(gCamera &m_camera, gShaderProgram &m_program)
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

	for (int i = 0; i < (7 * 7); i++) {
		enemyPlayTiles[i].Draw(m_camera, m_program);
		myPlayTiles[i].Draw(m_camera, m_program);
	}

}

PlayTile * Sea::getMyTiles()
{
	return myPlayTiles;
}

PlayTile * Sea::getEnemyTiles()
{
	return enemyPlayTiles;
}

