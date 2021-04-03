#include "Fleet.h"

#include <iostream>
Fleet::Fleet(void)
{
	
}

Fleet::~Fleet(void)
{
}

void Fleet::Init(std::pair<char,int>* actPlayTiles)
{
	float tile_transX = 0;
	float tile_transZ = 0;
	glm::vec3 tileResult = glm::vec3(0.0f);
	for (int i = 0; i < 16; i++) {
		//std::cout << actPlayTiles[i] << "fsfa" << std::endl;

		tile_transX = (actPlayTiles[i].second - 1) * 2.0f * TorpedoGLOBAL::Scale;
		tile_transZ = ((actPlayTiles[i].first - 'a') * 2.0f * TorpedoGLOBAL::Scale) - (3.0f*2.0f* TorpedoGLOBAL::Scale);

		tileResult += glm::vec3(mountaincenter_border_Xoffset, 0, 0) +
			glm::vec3(mountain_tile_offset, 0, 0) +
			glm::vec3(tile_transX, 0, tile_transZ);

		myShips[i] = Ship(glm::vec3(-1, 1, 1)*tileResult);
		//enemyShips[i] = Ship(tileResult);

		//enemyShips[i].Init();
		//myShips[i].Init();
		tileResult = glm::vec3(0.0f);
	}

	glm::vec3 battleShipOffset = glm::vec3(firstTile_battleShipOffset, 0, 0) + glm::vec3(mountaincenter_border_Xoffset + mountain_tile_offset, 0, 0);
	myBattleShip = Ship(-battleShipOffset);
	//myBattleShip.Init();
	enemyBattleShip = Ship(battleShipOffset);

	myBattleShip.Init(false);
	enemyBattleShip.Init(true);
	for (int i = 0; i < 16; i++) {
		myShips[i].Init(false);
		//enemyShips[i].Init(true);
	}
}

void Fleet::Draw(gCamera &camera, gShaderProgram &sh_program)
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

	for (int i = 0; i < 16; i++) {
		myShips[i].Draw(camera,sh_program);
		//enemyShips[i].Draw(m_camera,m_program);
	}
	myBattleShip.Draw(camera, sh_program);
	enemyBattleShip.Draw(camera,sh_program);

}