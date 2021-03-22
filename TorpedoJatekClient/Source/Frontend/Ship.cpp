
#include "Ship.h"

#include "../../Utils/gCamera.h"
#include "../../Utils/gShaderProgram.h"


Ship::Ship(void)
{

}

Ship::Ship(glm::vec3 fleetTranslate)
{
	ship_translate = fleetTranslate;
}

Ship::Ship(int tilePos)
{

}


Ship::~Ship(void)
{
}

void Ship::Init(bool isEnemy)
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
	
	if (isEnemy) {
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
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
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