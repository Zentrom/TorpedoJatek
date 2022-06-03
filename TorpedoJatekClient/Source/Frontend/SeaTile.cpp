
#include "SeaTile.h"

const float SeaTile::scaleXZ = 2.0f; //ez most 1 és 2-re megy

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
	vb_seatile.AddAttribute(0, 3); //pozíció
	vb_seatile.AddAttribute(1, 4); //szín+transparency
	vb_seatile.AddAttribute(2, 3); //normálvektor
	vb_seatile.AddAttribute(3, 2); //textúra koord

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

	vb_seatile.AddData(3, 0, 0);
	vb_seatile.AddData(3, 1, 0);
	vb_seatile.AddData(3, 0, 1);
	vb_seatile.AddData(3, 1, 1);

	vb_seatile.AddIndex(1, 0, 2);
	vb_seatile.AddIndex(1, 2, 3);

	vb_seatile.InitBuffers();
}

//Kirajzol egy tengermezõt
void SeaTile::Draw(gCamera &camera, gShaderProgram &sh_program, GLuint &textureID)
{
	glm::mat4 matWorld = glm::translate(seatile_translate) * glm::rotate(seatile_rotate, seatile_rotate_angle) * glm::scale(seatile_scale);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);

	sh_program.SetUniform("is_seatile", true);
	sh_program.SetUniform("hasTexture", true);
	sh_program.SetTexture("texImage", 0, textureID);

	vb_seatile.On();
	vb_seatile.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	vb_seatile.Off();

	sh_program.SetUniform("is_seatile", false);
	sh_program.SetUniform("hasTexture", false);
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
