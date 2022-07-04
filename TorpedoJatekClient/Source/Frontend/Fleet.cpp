#include "Fleet.h"

Fleet::Fleet(bool ally) : isAlly(ally)
{
}

Fleet::~Fleet()
{
	delete battleShip;
	for (Ship* sh : ships) {
		delete sh;
	}
}

//Inicializálja egy játékos hajóseregét
void Fleet::Init(int in_map_size, float pt_center_offset)
{
	mapSize = in_map_size;

	switch (mapSize) {
	case 5:
		pUnplacedShipCount = &ShipCount::Five;
		break;
	case 7:
		pUnplacedShipCount = &ShipCount::Seven;
		break;
	case 9:
		pUnplacedShipCount = &ShipCount::Nine;
		break;
	}

	float firstTileBattleShipOffset = ((TorpedoGLOBAL::SeaTileScaleXZ * mapSize) + bShipPlayZoneOffset) * TorpedoGLOBAL::Scale;
	glm::vec3 battleShipOffset = glm::vec3(firstTileBattleShipOffset, 0, 0) + glm::vec3(pt_center_offset, 0, 0);
	battleShip = new BattleShip(battleShipOffset, isAlly);
}

//Elkéri a játékos játékmezõit
void Fleet::InitTiles(std::vector<PlayTile*> &tiles)
{
	pPlayTiles = &tiles;
}

//Fleet adatok frissítése real-time
void Fleet::Update(float delta_time)
{
	for (Ship* sh : ships) {
		sh->Update(delta_time);
	}
	battleShip->Update(delta_time);
}

//Játékos hajóinak kirajzolása
void Fleet::Draw(const gCamera& camera, gShaderProgram& sh_program) const
{
	for (Ship* ship : ships) {
		if (ship->isVisible()) {
			ship->Draw(camera, sh_program);
		}
	}
	if (battleShip->isVisible()) {
		battleShip->Draw(camera, sh_program);
	}
}

//Megnezi hogy szabad-e a játékmezõ
bool Fleet::CheckTile(const PlayTile& tile) const
{
	for (PlayTile* &playTile : *pPlayTiles) {
		if (tile.getPos() == playTile->getPos()) {
			if (playTile->isUsed()) {
				return false;
			}
		}
	}
	return true;
}

//Koordináta alapján visszaad egy játékmezõt
PlayTile& Fleet::getTile(const std::pair<char, int> &pos)
{
	for (PlayTile* &playTile : *pPlayTiles) {
		if (pos == playTile->getPos()) {
			return *playTile;
		}
	}
	return *pPlayTiles->at(0);
}

//Visszaadja azokat a mezõket,ahova egy hajó másik vége lerakható lenne
std::array<PlayTile*, 4> Fleet::getFreeBacks(const PlayTile &tile, int back_distance) const
{
	std::array<PlayTile*, 4> result;
	bool rightFree = true;
	bool leftFree = true;
	bool upFree = true;
	bool downFree = true;

	if (((tile.getIndex() % mapSize) + back_distance) >= mapSize) {
		rightFree = false;
	}
	if (((tile.getIndex() % mapSize) - back_distance) < 0) {
		leftFree = false;
	}
	if (((tile.getIndex() / mapSize) + back_distance) >= mapSize) {
		downFree = false;
	}
	if (((tile.getIndex() / mapSize) - back_distance) < 0) {
		upFree = false;
	}

	for (int i = back_distance; i > 0; --i) {
		if (rightFree) {
			if (pPlayTiles->at(tile.getIndex() + i)->isUsed()) {
				rightFree = false;
			}
		}
		if (leftFree) {
			if (pPlayTiles->at(tile.getIndex() - i)->isUsed()) {
				leftFree = false;
			}
		}
		if (downFree) {
			if (pPlayTiles->at(tile.getIndex() + i*mapSize)->isUsed()) {
				downFree = false;
			}
		}
		if (upFree) {
			if (pPlayTiles->at(tile.getIndex() - i*mapSize)->isUsed()) {
				upFree = false;
			}
		}
	}

	if (rightFree) {
		result[0] = pPlayTiles->at(tile.getIndex() + back_distance);
	}
	else {
		result[0] = nullptr;
	}
	if (leftFree) {
		result[1] = pPlayTiles->at(tile.getIndex() - back_distance);
	}
	else {
		result[1] = nullptr;
	}
	if (downFree) {
		result[2] = pPlayTiles->at(tile.getIndex() + back_distance * mapSize);
	}
	else {
		result[2] = nullptr;
	}
	if (upFree) {
		result[3] = pPlayTiles->at(tile.getIndex() - back_distance * mapSize);
	}
	else {
		result[3] = nullptr;
	}

	return result;
}

//Lerak egy hajót a pályára
void Fleet::PlaceShip(PlayTile *front, PlayTile *back)
{
	if (TorpedoGLOBAL::Debug) {
		std::cout << "-------------------------------------" << std::endl;
	}

	std::vector<PlayTile*> shipTiles;

	if (front) {
		PlayTile* tmpTile = pPlayTiles->at(front->getIndex());
		if (!back) {
			shipTiles.push_back(tmpTile);
		}
		else if (front->getPos().first == back->getPos().first)
		{
			while (tmpTile->getIndex() > back->getIndex()) {
				shipTiles.push_back(tmpTile);
				tmpTile = pPlayTiles->at(tmpTile->getIndex() - 1);
			}
			while (tmpTile->getIndex() < back->getIndex()) {
				shipTiles.push_back(tmpTile);
				tmpTile = pPlayTiles->at(tmpTile->getIndex() + 1);
			}
			shipTiles.push_back(back);
		}
		else if (front->getPos().first != back->getPos().first) {
			if (front->getPos().second != back->getPos().second) {
				std::cout << "[Fleet_PlaceShip]Error while placing ship: Input column and row both mismatch!" << std::endl;
				return;
			}
			else {
				while (tmpTile->getIndex() > back->getIndex()) {
					shipTiles.push_back(tmpTile);
					tmpTile = pPlayTiles->at(tmpTile->getIndex() - mapSize);
				}
				while (tmpTile->getIndex() < back->getIndex()) {
					shipTiles.push_back(tmpTile);
					tmpTile = pPlayTiles->at(tmpTile->getIndex() + mapSize);
				}
			}
			shipTiles.push_back(back);
		}

		for (PlayTile *til : shipTiles) {
			if (TorpedoGLOBAL::Debug) {
				std::cout << "Fleet::PlaceShip(): " << til->getIndex() << " ally? " << isAlly << std::endl;
			}
			til->setUsed(true);
		}
		ships.push_back(new Ship(shipTiles, isAlly));
	}
}

//Lekezel ellenséges lövést
void Fleet::HitFleet(std::pair<char, int> hit_pos)
{
	bool found = false;
	for (Ship* sh : ships) {
		if (!sh->isDestroyed()) {
			for (PlayTile* &pt : sh->getPlayTiles()) {
				if (pt && pt->getPos() == hit_pos) {
					pt = nullptr;
					found = true;
					break;
				}
			}
			bool notAll = false;
			for (PlayTile* &pt : sh->getPlayTiles()) {
				if (pt) {
					notAll = true;
					break;
				}
			}
			if (!notAll) {
				sh->setDestroyed(true);
			}
			if (found) {
				break;
			}
		}
	}
}

//Visszaadja a le nem rakott hajók számát méret alapján(1x1,2x2,stb.)
const std::array<int, 4>* Fleet::getUnplacedShipCount() const
{
	return pUnplacedShipCount;
}

//Visszaadja azoknak a mezõknek a koordinátáit amin van hajó, kilõttek is akár
std::vector<std::pair<char, int>> Fleet::getActiveTilePositions() const
{
	std::vector<std::pair<char, int>> result;
	for (Ship* ship : ships) {
		for (PlayTile* tile : ship->getPlayTiles()) {
			if (tile) {
				result.push_back(tile->getPos());
			}
		}
	}
	return result;
}

BattleShip& Fleet::getBattleShip()
{
	return *battleShip;
}

const bool Fleet::getIsAlly() const
{
	return isAlly;
}