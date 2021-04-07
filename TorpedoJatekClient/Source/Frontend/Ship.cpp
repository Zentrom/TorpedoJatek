
#include "Ship.h"

#include <iostream>

Ship::Ship(void)
{

}

Ship::Ship(bool isBattleShip,bool ally,glm::vec3 battleShipTranslate)
{
	isAlly = ally;
	Init(isAlly);

	ship_translate = glm::vec3((ally?-1:1) ,1,1) * (battleShipTranslate+glm::vec3(1,0,0)) * TorpedoGLOBAL::Scale;

	//std::cout << "Battlesh: " << ship_translate.x << ' ' << ship_translate.y << ' ' << ship_translate.z << std::endl;
}

Ship::Ship(std::vector<PlayTile*> tiles,bool ally)
{
	isAlly = ally;
	Init(isAlly);

	playTiles = tiles;

	glm::vec3 frontTranslation = tiles[0]->getTranslate();
	glm::vec3 backTranslation = tiles[tiles.size() - 1]->getTranslate();
	ship_translate = (frontTranslation + backTranslation) / 2.0f;

	ship_scale = glm::vec3(1.6f*tiles.size(), 0.6f + 0.4f*tiles.size(), 0.6f + 0.25f*tiles.size()) * TorpedoGLOBAL::Scale;
	if (tiles.size() > 1) {
		if (tiles[0]->getPos().first != tiles[1]->getPos().first) {
			ship_rotate = glm::half_pi<float>();
		}
	}
}

Ship::~Ship(void)
{
}

void Ship::Init(bool isAlly)
{
	vb_ship.AddAttribute(0, 3);
	vb_ship.AddAttribute(1, 3);
	   
	vb_ship.AddData(0, -0.5f, -0.5f, 0.5f);
	vb_ship.AddData(0, 0.5f, -0.5f, 0.5f);
	vb_ship.AddData(0, -0.5f, 0.5f, 0.5f);
	vb_ship.AddData(0, 0.5f, 0.5f, 0.5f);
	vb_ship.AddData(0, -0.5f, -0.5f, -0.5f);
	vb_ship.AddData(0, 0.5f, -0.5f, -0.5f);
	vb_ship.AddData(0, -0.5f, 0.5f, -0.5f);
	vb_ship.AddData(0, 0.5f, 0.5f, -0.5f);
	
	if (!isAlly) {
		vb_ship.AddData(1, 1.0f, 0, 0);
		vb_ship.AddData(1, 1.0f, 0, 0);
		vb_ship.AddData(1, 1.0f, 0, 0);
		vb_ship.AddData(1, 1.0f, 0, 0);
		vb_ship.AddData(1, 1.0f, 0, 0);
		vb_ship.AddData(1, 1.0f, 0, 0);
		vb_ship.AddData(1, 1.0f, 0, 0);
		vb_ship.AddData(1, 1.0f, 0, 0);
	}
	else {
		vb_ship.AddData(1, 0, 1.0f, 0);
		vb_ship.AddData(1, 0, 1.0f, 0);
		vb_ship.AddData(1, 0, 1.0f, 0);
		vb_ship.AddData(1, 0, 1.0f, 0);
		vb_ship.AddData(1, 0, 1.0f, 0);
		vb_ship.AddData(1, 0, 1.0f, 0);
		vb_ship.AddData(1, 0, 1.0f, 0);
		vb_ship.AddData(1, 0, 1.0f, 0);
	}
	   
	vb_ship.AddIndex(1, 2, 0);
	vb_ship.AddIndex(1, 3, 2);
	vb_ship.AddIndex(5, 3, 1);
	vb_ship.AddIndex(5, 7, 3);
	vb_ship.AddIndex(4, 7, 5);
	vb_ship.AddIndex(4, 6, 7);
	vb_ship.AddIndex(0, 6, 4);
	vb_ship.AddIndex(0, 2, 6);
	vb_ship.AddIndex(3, 6, 2);
	vb_ship.AddIndex(3, 7, 6);
	vb_ship.AddIndex(5, 0, 4);
	vb_ship.AddIndex(5, 1, 0);
	   
	vb_ship.InitBuffers();
}

void Ship::Draw(gCamera &camera, gShaderProgram &sh_program)
{

	glm::mat4 matWorld = glm::translate(ship_translate) * glm::rotate(ship_rotate, ship_rotate_angle) * glm::scale(ship_scale);
	//glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	//sh_program.SetUniform("world", matWorld);
	//sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);

	//m_program.SetTexture("texImage", 0, m_groundTextureID);
	//m_program.SetTexture("texNormal", 1, m_groundNormalMapID);

	vb_ship.On();
	vb_ship.DrawIndexed(GL_TRIANGLES, 0, 36, 0);
	vb_ship.Off();
}

std::vector<PlayTile*> Ship::getPlayTiles()
{
	return playTiles;
}
