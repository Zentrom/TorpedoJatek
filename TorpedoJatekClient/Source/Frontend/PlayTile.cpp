
#include "PlayTile.h"

PlayTile::PlayTile(void)
{
}

//PlayTile::PlayTile(glm::vec3 translate)
//{
//	seatile_translate = translate;
//}

PlayTile::PlayTile(std::pair<char, int> pos)
{
	position = pos;
}

PlayTile::~PlayTile(void)
{
}

void PlayTile::Draw(gCamera &camera,gShaderProgram &sh_playtile)
{

	glm::mat4 matWorld = glm::translate(seatile_translate) * glm::rotate(seatile_rotate, seatile_rotate_angle) * glm::scale(seatile_scale);
	//glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	//sh_playtile.SetUniform("world", matWorld);
	//sh_playtile.SetUniform("worldIT", matWorldIT);
	sh_playtile.SetUniform("MVP", mvp);
	sh_playtile.SetUniform("tile_state", tileState);

	//m_program.SetTexture("texImage", 0, m_groundTextureID);
	//m_program.SetTexture("texNormal", 1, m_groundNormalMapID);

	vb_seatile.On();
	vb_seatile.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	vb_seatile.Off();

}

void PlayTile::setState(int newState)
{
	switch (newState) {
	case 1:
		tileState = glm::vec3(1, 0, 0);
		break;
	case 2:
		tileState = glm::vec3(1, 1, 0);
		break;
	case 3:
		tileState = glm::vec3(0, 0, 1);
		break;
	case 4:
		tileState = glm::vec3(1, 0, 0);
		break;
	case 5:
		tileState = glm::vec3(1, 0, 0);
		break;
	default:
		tileState = glm::vec3(0, 0, 1);
		break;
	}
}

void PlayTile::setIndex(int newIndex)
{
	index = newIndex;
}

void PlayTile::setUsed(bool used)
{
	usedTile = used;
}

int PlayTile::getIndex()
{
	return index;
}

std::pair<char, int> PlayTile::getPos()
{
	return position;
}

bool PlayTile::isUsed()
{
	return usedTile;
}

glm::vec3 PlayTile::getTranslate()
{
	return seatile_translate;
}
