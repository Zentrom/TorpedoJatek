#include "Fleet.h"

Fleet::Fleet(void)
{
}

Fleet::~Fleet(void)
{
}

//Inicializálja egy játékos hajóseregét
void Fleet::Init(int inMapSize, bool ally)
{
	mapSize = inMapSize;
	isAlly = ally;

	switch (mapSize) {
	case 5:
		unplacedShipCount = ShipCount::Five;
		break;
	case 7:
		unplacedShipCount = ShipCount::Seven;
		break;
	case 9:
		unplacedShipCount = ShipCount::Nine;
		break;
	}

	firstTile_battleShipOffset = ((2.0f * mapSize) + 1.0f) * TorpedoGLOBAL::Scale;
	mountaincenter_border_Xoffset = (mapSize - 1) / 2.0f * TorpedoGLOBAL::Scale;

	glm::vec3 battleShipOffset = glm::vec3(firstTile_battleShipOffset, 0, 0) + glm::vec3(mountaincenter_border_Xoffset + mountain_tile_offset, 0, 0);
	battleShip = Ship(isAlly, battleShipOffset);
}

//Elkéri a játékos játékmezõit
void Fleet::InitTiles(const std::vector<PlayTile> &tiles)
{
	playTiles = tiles;
}

//Játékos hajóinak kirajzolása
void Fleet::Draw(gCamera &camera, gShaderProgram &sh_program)
{

	for (Ship &ship : ships) {
		ship.Draw(camera, sh_program);
	}
	battleShip.Draw(camera, sh_program);

}

//Megnezi hogy szabad-e a játékmezõ
bool Fleet::CheckTile(const PlayTile &tile)
{
	for (PlayTile &playTile : playTiles) {
		if (tile.getPos() == playTile.getPos() && playTile.isUsed()) {
			return false;
		}
	}
	return true;
}

//Koordináta alapján visszaad egy játékmezõt
PlayTile& Fleet::getTile(const std::pair<char, int> &pos)
{
	for (PlayTile &playTile : playTiles) {
		if (pos == playTile.getPos()) {
			return playTile;
		}
	}
	return playTiles[0];
}

//Visszaadja azokat a mezõket,ahova egy hajó másik vége lerakható lenne
std::array<PlayTile*, 4> Fleet::getFreeBacks(const PlayTile &tile, int backDistance)
{
	std::array<PlayTile*, 4> result;
	bool rightFree = true;
	bool leftFree = true;
	bool upFree = true;
	bool downFree = true;

	if (((tile.getIndex() % mapSize) + backDistance) >= mapSize) {
		rightFree = false;
	}
	if (((tile.getIndex() % mapSize) - backDistance) < 0) {
		leftFree = false;
	}
	if (((tile.getIndex() / mapSize) + backDistance) >= mapSize) {
		downFree = false;
	}
	if (((tile.getIndex() / mapSize) - backDistance) < 0) {
		upFree = false;
	}

	for (int i = backDistance; i > 0; i--) {
		if (rightFree) {
			if (playTiles.at(tile.getIndex() + i).isUsed()) {
				rightFree = false;
			}
		}
		if (leftFree) {
			if (playTiles.at(tile.getIndex() - i).isUsed()) {
				leftFree = false;
			}
		}
		if (downFree) {
			if (playTiles.at(tile.getIndex() + i*mapSize).isUsed()) {
				downFree = false;
			}
		}
		if (upFree) {
			if (playTiles.at(tile.getIndex() - i*mapSize).isUsed()) {
				upFree = false;
			}
		}
	}

	if (rightFree) {
		result[0] = &playTiles.at(tile.getIndex() + backDistance);
	}
	else {
		result[0] = nullptr;
	}
	if (leftFree) {
		result[1] = &playTiles.at(tile.getIndex() - backDistance);
	}
	else {
		result[1] = nullptr;
	}
	if (downFree) {
		result[2] = &playTiles.at(tile.getIndex() + backDistance*mapSize);
	}
	else {
		result[2] = nullptr;
	}
	if (upFree) {
		result[3] = &playTiles.at(tile.getIndex() - backDistance*mapSize);
	}
	else {
		result[3] = nullptr;
	}

	return result;
}

//Lerak egy hajót a pályára
void Fleet::PlaceShip(PlayTile *front, PlayTile *back)
{
	std::vector<PlayTile*> shipTiles;

	if (front) {
		PlayTile* tmpTile = &playTiles[front->getIndex()];
		if (!back) {
			shipTiles.push_back(tmpTile);
		}
		else if (front->getPos().first == back->getPos().first)
		{
			while (tmpTile->getIndex() > back->getIndex()) {
				shipTiles.push_back(tmpTile);
				tmpTile--;
			}
			while (tmpTile->getIndex() < back->getIndex()) {
				shipTiles.push_back(tmpTile);
				tmpTile++;
			}
			shipTiles.push_back(back);
		}
		else if (front->getPos().first != back->getPos().first) {
			while (tmpTile->getIndex() > back->getIndex()) {
				shipTiles.push_back(tmpTile);
				tmpTile -= mapSize;
			}
			while (tmpTile->getIndex() < back->getIndex()) {
				shipTiles.push_back(tmpTile);
				tmpTile += mapSize;
			}
			shipTiles.push_back(back);
		}

		for (PlayTile *tile : shipTiles) {
			tile->setUsed(true);
		}
		ships.push_back(Ship(shipTiles, isAlly));
	}
}

//Visszaadja a le nem rakott hajók számát méret alapján(1x1,2x2,stb.)
std::array<int, 4>& Fleet::getUnplacedShipCount()
{
	return unplacedShipCount;
}

//Visszaadja azoknak a mezõknek a koordinátáit amin van hajó
std::vector<std::pair<char, int>> Fleet::getActiveTilePositions()
{
	std::vector<std::pair<char, int>> result;
	for (Ship &ship : ships) {
		for (PlayTile* tile : ship.getPlayTiles()) {
			result.push_back(tile->getPos());
		}
	}
	return result;
}
