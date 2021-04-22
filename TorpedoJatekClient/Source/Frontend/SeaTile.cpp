
#include "SeaTile.h"

const float SeaTile::scaleXZ = 2.0f;

SeaTile::SeaTile(void)
{
	Init();
}

SeaTile::SeaTile(const glm::vec3 &trans)
{
	seatile_translate += trans;
	Init();
}

SeaTile::~SeaTile(void)
{
}

//Inicializálja a tengermezõ grafikai modelljét
void SeaTile::Init()
{
	vb_seatile.AddAttribute(0, 3);
	vb_seatile.AddAttribute(1, 4);
	vb_seatile.AddAttribute(2, 3);

	vb_seatile.AddData(0, -0.5f, 0, -0.5f);
	vb_seatile.AddData(0, 0.5f, 0, -0.5f);
	vb_seatile.AddData(0, -0.5f, 0, 0.5f);
	vb_seatile.AddData(0, 0.5f, 0, 0.5f);

	vb_seatile.AddData(1, 0, 0, 1.0f, 0.5f);
	vb_seatile.AddData(1, 0, 0, 1.0f, 0.5f);
	vb_seatile.AddData(1, 0, 0, 1.0f, 0.5f);
	vb_seatile.AddData(1, 0, 0, 1.0f, 0.5f);

	vb_seatile.AddData(2, 0, 1.0f, 0);
	vb_seatile.AddData(2, 0, 1.0f, 0);
	vb_seatile.AddData(2, 0, 1.0f, 0);
	vb_seatile.AddData(2, 0, 1.0f, 0);

	vb_seatile.AddIndex(1, 0, 2);
	vb_seatile.AddIndex(1, 2, 3);

	vb_seatile.InitBuffers();
}

//Kirajzol egy tengermezõt
void SeaTile::Draw(gCamera &camera, gShaderProgram &sh_playtile)
{
	glm::mat4 matWorld = glm::translate(seatile_translate) * glm::rotate(seatile_rotate, seatile_rotate_angle) * glm::scale(seatile_scale);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	sh_playtile.SetUniform("world", matWorld);
	sh_playtile.SetUniform("worldIT", matWorldIT);
	sh_playtile.SetUniform("MVP", mvp);
	//sh_playtile.SetUniform("tile_state", glm::vec3(0, 0, 1));

	//m_program.SetTexture("texImage", 0, m_groundTextureID);
	//m_program.SetTexture("texNormal", 1, m_groundNormalMapID);

	vb_seatile.On();
	vb_seatile.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	vb_seatile.Off();
}

void SeaTile::setTranslate(const glm::vec3 &trans)
{
	seatile_translate = trans;
}

//Visszaadja a tengermezõ skálázását XZ tengely mentén
float SeaTile::getScaleXZ()
{
	return scaleXZ;
}

glm::vec3 SeaTile::getTranslate() const
{
	return seatile_translate;
}
