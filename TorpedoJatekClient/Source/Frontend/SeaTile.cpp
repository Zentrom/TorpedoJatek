
#include "SeaTile.h"

const float SeaTile::scaleXZ = 2.0f; //ez most 1 �s 2-re megy

//SeaTile::SeaTile()
//{
//	Init();
//}

SeaTile::SeaTile(const glm::vec3 &trans) : seatileTranslate(trans),
	matWorld(glm::translate(seatileTranslate) * glm::scale(seatileScale)),
	matWorldIT(glm::transpose(glm::inverse(matWorld)))
{
	Init();
}

SeaTile::~SeaTile()
{
	//vb_seatile.Clean();
}

//Inicializ�lja a tengermez� grafikai modellj�t
void SeaTile::Init()
{
	vb_seatile.AddAttribute(0, 3); //poz�ci�
	vb_seatile.AddAttribute(1, 4); //sz�n+transparency
	vb_seatile.AddAttribute(2, 3); //norm�lvektor
	vb_seatile.AddAttribute(3, 2); //text�ra koord

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

//Kirajzol egy tengermez�t
void SeaTile::Draw(const gCamera& camera, gShaderProgram& sh_program) const
{
	glm::mat4 mvp = camera.GetViewProj() * matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);

	vb_seatile.On();
	vb_seatile.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	vb_seatile.Off();

}

void SeaTile::setTranslate(const glm::vec3 &trans)
{
	seatileTranslate = trans;

	matWorld = glm::translate(seatileTranslate) * glm::scale(seatileScale);
	matWorldIT = glm::transpose(glm::inverse(matWorld));
}

//Visszaadja a tengermez� sk�l�z�s�t XZ tengely ment�n
float SeaTile::getScaleXZ()
{
	return scaleXZ;
}

const glm::vec3& SeaTile::getTranslate() const
{
	return seatileTranslate;
}
