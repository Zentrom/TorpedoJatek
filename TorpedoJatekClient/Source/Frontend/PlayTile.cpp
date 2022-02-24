
#include "PlayTile.h"

PlayTile::PlayTile(void)
{
}

PlayTile::PlayTile(const std::pair<char, int> &pos) : position(pos)
{
}

PlayTile::~PlayTile(void)
{
}

//Játékmezõ indexét rajzoljuk ki az alpha színcsatornába
void PlayTile::PreProcess(gCamera &camera, gShaderProgram &sh_program)
{
	glm::mat4 matWorld = glm::translate(seatile_translate) * glm::rotate(seatile_rotate, seatile_rotate_angle) * glm::scale(seatile_scale);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	sh_program.SetUniform("is_preprocess", true);
	sh_program.SetUniform("tile_index", index + indexOffset);

	vb_seatile.On();
	vb_seatile.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	vb_seatile.Off();
}

//Egy játékmezõ kirajzolása
void PlayTile::Draw(gCamera &camera, gShaderProgram &sh_program, GLuint &textureID)
{

	glm::mat4 matWorld = glm::translate(seatile_translate) * glm::rotate(seatile_rotate, seatile_rotate_angle) * glm::scale(seatile_scale - outlineWidth);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	sh_program.SetUniform("is_seatile", true);
	sh_program.SetUniform("tilestate_changed", isStateChanged);
	sh_program.SetUniform("tile_state", tileState);
	//sh_program.SetUniform("tile_index", index + indexOffset);

	sh_program.SetUniform("hasTexture", true);
	sh_program.SetTexture("texImage", 0, textureID);

	vb_seatile.On();
	vb_seatile.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	vb_seatile.Off();

	sh_program.SetUniform("hasTexture", false);
	sh_program.SetUniform("is_seatile", false);
	sh_program.SetUniform("tilestate_changed", false);
}

void PlayTile::OutlineDraw(gCamera &camera, gShaderProgram &sh_program)
{
	glm::mat4 matWorld = glm::translate(seatile_translate) * glm::rotate(seatile_rotate, seatile_rotate_angle) * glm::scale(seatile_scale);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	sh_program.SetUniform("is_preprocess", false);
	sh_program.SetUniform("tile_state", tileState);
	sh_program.SetUniform("tile_index", index + indexOffset);

	vb_seatile.On();
	vb_seatile.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	vb_seatile.Off();
}

//Játékmezõ állapotának állítása(hogy más színe legyen)
void PlayTile::setState(int newState)
{
	isStateChanged = true;
	switch (newState) {
	case 1: //hit
		tileState = glm::vec3(1, 0, 0);
		break;
	case 2: //miss
		if (tileState.x != 1 || tileState.y != 0 || tileState.z != 0) {
			tileState = glm::vec3(1, 1, 0);
		}
		break;
	case 3: //default
		tileState = glm::vec3(1, 1, 1);
		isStateChanged = false;
		break;
	case 4: //winPlayerOne
		tileState = glm::vec3(1, 0, 0);
		break;
	case 5: //winPlayerTwo
		tileState = glm::vec3(1, 0, 0);
		break;
	case 6: //Back of the ship placeable here
		tileState = glm::vec3(0, 1, 0);
		break;
	case 7: //Cursor selection color
		tileState = glm::vec3(0, 1, 1);
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

//Kinullázza a játékmezõ koordinátáját
void PlayTile::ClearPosition()
{
	position = std::pair<char, int>('0', 0);
}

int PlayTile::getIndex() const
{
	return index;
}

std::pair<char, int> PlayTile::getPos() const
{
	return position;
}

bool PlayTile::isUsed() const
{
	return usedTile;
}

glm::vec3 PlayTile::getTranslate() const
{
	return seatile_translate;
}

int PlayTile::getIndexOffset() const
{
	return indexOffset;
}

glm::vec3 PlayTile::getState() const
{
	return tileState;
}
