
#include "PlayTile.h"

//PlayTile::PlayTile()
//{
//}

PlayTile::PlayTile(const std::pair<char, int>& pos, const glm::vec3& trans) : SeaTile::SeaTile(trans), position(pos)
{
}

PlayTile::~PlayTile()
{
}

//Játékmezõ indexét rajzoljuk ki az alpha színcsatornába
void PlayTile::PreProcess(const gCamera& camera, gShaderProgram& sh_program) const
{
	//glm::mat4 matWorldd = glm::translate(seatileTranslate) * glm::scale(seatileScale);
	//glm::mat4 matWorldITd = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() * matWorld;
	
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
void PlayTile::Draw(const gCamera& camera, gShaderProgram& sh_program) const
{

	glm::mat4 matWorld = glm::translate(seatileTranslate) * glm::scale(seatileScale - outlineWidth);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() * matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	//sh_program.SetUniform("is_seatile", true);
	sh_program.SetUniform("tilestate_changed", isStateChanged);
	sh_program.SetUniform("tile_state", stateColor);

	vb_seatile.On();
	vb_seatile.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	vb_seatile.Off();

	//sh_program.SetUniform("hasTexture", false);
	//sh_program.SetUniform("is_seatile", false);
	sh_program.SetUniform("tilestate_changed", false);
}

//Körvonalakat rajzolja ki a játékmezõk köré
void PlayTile::OutlineDraw(const gCamera& camera, gShaderProgram& sh_program) const
{
	//glm::mat4 matWorld = glm::translate(seatileTranslate) *  glm::scale(seatileScale);
	//glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() * matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	//sh_program.SetUniform("is_preprocess", false);
	sh_program.SetUniform("tile_state", stateColor);
	sh_program.SetUniform("tile_index", index + indexOffset);

	vb_seatile.On();
	vb_seatile.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	vb_seatile.Off();
}

//Játékmezõ állapotának állítása
void PlayTile::setState(int new_state)
{
	isStateChanged = true;
	if (new_state == 2) {
		//if (state == HIT) {             EZ AZ UJRASZINEZÉS HA MÁR LÖTTÜNK ODA???
			state = MISS;
		//}
	}
	else if (new_state == 3) {
		state = static_cast<TileState>(new_state);
		isStateChanged = false;
	}
	else {
		//EZ LEHET SZAR HA NAGYOBB JÖN BE MINT AZ ENUMHATÁR
		state = static_cast<TileState>(new_state);
	}
	setStateColor();
}

void PlayTile::setStateColor() 
{
	switch (state) {
	case 1: //hit
		stateColor = glm::vec3(1, 0, 0);
		break;
	case 2: //miss
		stateColor = glm::vec3(1, 1, 0);
		break;
	case 3: //default
		stateColor = glm::vec3(1, 1, 1);
		break;
	case 4: //winPlayerOne
		stateColor = glm::vec3(1, 0, 0);
		break;
	case 5: //winPlayerTwo
		stateColor = glm::vec3(1, 0, 0);
		break;
	case 6: //Back of the ship placeable here
		stateColor = glm::vec3(0, 1, 0);
		break;
	case 7: //Cursor selection color
		stateColor = glm::vec3(0, 1, 1);
		break;
	default:
		stateColor = glm::vec3(0, 0, 1);
		break;
	}
}

void PlayTile::setIndex(int new_index)
{
	index = new_index;
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

const int PlayTile::getIndex() const
{
	return index;
}

const std::pair<char, int> PlayTile::getPos() const
{
	return position;
}

//Van-e hajó a mezõn
bool PlayTile::isUsed() const
{
	return usedTile;
}

int PlayTile::getIndexOffset() const
{
	return indexOffset;
}

const glm::vec3& PlayTile::getStateColor() const
{
	return stateColor;
}
