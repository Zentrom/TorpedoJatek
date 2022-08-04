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
	vb_ship.Clean();
	vb_flag.Clean();
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

	InitShipVb();
	InitShipFlagVb();

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
			ship->Draw(camera, sh_program, vb_ship, vb_flag);
		}
	}
	if (battleShip->isVisible()) {
		battleShip->Draw(camera, sh_program, vb_ship, vb_flag);
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
void Fleet::HitFleet(const std::pair<char, int> hit_pos)
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
				sh->setShotAt(battleShip->getCannon().getProjectile().getAnimationTime());
			}
			if (found) {
				break;
			}
		}
	}
}

//Hajó modell adatainak inicializálása
void Fleet::InitShipVb()
{
	vb_ship.AddAttribute(0, 3); //pozíció
	vb_ship.AddAttribute(1, 4); //szín
	vb_ship.AddAttribute(2, 3); //normálvektor
	vb_ship.AddAttribute(3, 2); //textúrakoord

	//Hajó alsó palást
	vb_ship.AddData(0, -0.5f, 0, 0);
	vb_ship.AddData(0, -0.25f, -0.25f, 0);
	vb_ship.AddData(0, -0.25f, 0, 0.25f);
	vb_ship.AddData(0, -0.25f, 0, 0.25f);
	vb_ship.AddData(0, -0.25f, -0.25f, 0);
	vb_ship.AddData(0, 0.25f, 0, 0.25f);
	vb_ship.AddData(0, 0.25f, 0, 0.25f);
	vb_ship.AddData(0, -0.25f, -0.25f, 0);
	vb_ship.AddData(0, 0.25f, -0.25f, 0);
	vb_ship.AddData(0, 0.25f, 0, 0.25f);
	vb_ship.AddData(0, 0.25f, -0.25f, 0);
	vb_ship.AddData(0, 0.5f, 0, 0);
	//palást másik oldala
	vb_ship.AddData(0, 0.5f, 0, 0);
	vb_ship.AddData(0, 0.25f, -0.25f, 0);
	vb_ship.AddData(0, 0.25f, 0, -0.25f);
	vb_ship.AddData(0, 0.25f, 0, -0.25f);
	vb_ship.AddData(0, 0.25f, -0.25f, 0);
	vb_ship.AddData(0, -0.25f, 0, -0.25f);
	vb_ship.AddData(0, -0.25f, 0, -0.25f);
	vb_ship.AddData(0, 0.25f, -0.25f, 0);
	vb_ship.AddData(0, -0.25f, -0.25f, 0);
	vb_ship.AddData(0, -0.25f, 0, -0.25f);
	vb_ship.AddData(0, -0.25f, -0.25f, 0);
	vb_ship.AddData(0, -0.5f, 0, 0);
	//Fedõlap
	vb_ship.AddData(0, -0.25f, 0, -0.25f);
	vb_ship.AddData(0, -0.5f, 0, 0);
	vb_ship.AddData(0, -0.25f, 0, 0.25f);
	vb_ship.AddData(0, -0.25f, 0, -0.25f);
	vb_ship.AddData(0, -0.25f, 0, 0.25f);
	vb_ship.AddData(0, 0.25f, 0, 0.25f);
	vb_ship.AddData(0, 0.25f, 0, 0.25f);
	vb_ship.AddData(0, 0.25f, 0, -0.25f);
	vb_ship.AddData(0, -0.25f, 0, -0.25f);
	vb_ship.AddData(0, 0.25f, 0, -0.25f);
	vb_ship.AddData(0, 0.25f, 0, 0.25f);
	vb_ship.AddData(0, 0.5f, 0, 0);
	//Gúlasátor
	vb_ship.AddData(0, -0.25f, 0, 0);
	vb_ship.AddData(0, 0, 0, 0.25f);
	vb_ship.AddData(0, 0, 0.25f, 0);
	vb_ship.AddData(0, 0, 0.25f, 0);
	vb_ship.AddData(0, 0, 0, 0.25f);
	vb_ship.AddData(0, 0.25f, 0, 0);
	vb_ship.AddData(0, 0.25f, 0, 0);
	vb_ship.AddData(0, 0, 0, -0.25f);
	vb_ship.AddData(0, 0, 0.25f, 0);
	vb_ship.AddData(0, 0, 0.25f, 0);
	vb_ship.AddData(0, 0, 0, -0.25f);
	vb_ship.AddData(0, -0.25f, 0, 0);

	if (!isAlly) {
		for (int i = 0; i < 48; ++i) {
			vb_ship.AddData(1, 1.0f, 0, 0, 1);
		}
	}
	else {
		for (int i = 0; i < 48; ++i) {
			vb_ship.AddData(1, 0, 1.0f, 0, 1);
		}
	}

	vb_ship.AddData(2, -0.5f, 0, 0);
	vb_ship.AddData(2, -0.25f, -0.25f, 0);
	vb_ship.AddData(2, -0.25f, 0, 0.25f);
	vb_ship.AddData(2, -0.25f, 0, 0.25f);
	vb_ship.AddData(2, -0.25f, -0.25f, 0);
	vb_ship.AddData(2, 0.25f, 0, 0.25f);
	vb_ship.AddData(2, 0.25f, 0, 0.25f);
	vb_ship.AddData(2, -0.25f, -0.25f, 0);
	vb_ship.AddData(2, 0.25f, -0.25f, 0);
	vb_ship.AddData(2, 0.25f, 0, 0.25f);
	vb_ship.AddData(2, 0.25f, -0.25f, 0);
	vb_ship.AddData(2, 0.5f, 0, 0);
	vb_ship.AddData(2, 0.5f, 0, 0);
	vb_ship.AddData(2, 0.25f, -0.25f, 0);
	vb_ship.AddData(2, 0.25f, 0, -0.25f);
	vb_ship.AddData(2, 0.25f, 0, -0.25f);
	vb_ship.AddData(2, 0.25f, -0.25f, 0);
	vb_ship.AddData(2, -0.25f, 0, -0.25f);
	vb_ship.AddData(2, -0.25f, 0, -0.25f);
	vb_ship.AddData(2, 0.25f, -0.25f, 0);
	vb_ship.AddData(2, -0.25f, -0.25f, 0);
	vb_ship.AddData(2, -0.25f, 0, -0.25f);
	vb_ship.AddData(2, -0.25f, -0.25f, 0);
	vb_ship.AddData(2, -0.5f, 0, 0);
	vb_ship.AddData(2, -0.25f, 0, -0.25f);
	vb_ship.AddData(2, -0.5f, 0, 0);
	vb_ship.AddData(2, -0.25f, 0, 0.25f);
	vb_ship.AddData(2, -0.25f, 0, -0.25f);
	vb_ship.AddData(2, -0.25f, 0, 0.25f);
	vb_ship.AddData(2, 0.25f, 0, 0.25f);
	vb_ship.AddData(2, 0.25f, 0, 0.25f);
	vb_ship.AddData(2, 0.25f, 0, -0.25f);
	vb_ship.AddData(2, -0.25f, 0, -0.25f);
	vb_ship.AddData(2, 0.25f, 0, -0.25f);
	vb_ship.AddData(2, 0.25f, 0, 0.25f);
	vb_ship.AddData(2, 0.5f, 0, 0);
	vb_ship.AddData(2, -0.25f, 0, 0);
	vb_ship.AddData(2, 0, 0, 0.25f);
	vb_ship.AddData(2, 0, 0.25f, 0);
	vb_ship.AddData(2, 0, 0.25f, 0);
	vb_ship.AddData(2, 0, 0, 0.25f);
	vb_ship.AddData(2, 0.25f, 0, 0);
	vb_ship.AddData(2, 0.25f, 0, 0);
	vb_ship.AddData(2, 0, 0, -0.25f);
	vb_ship.AddData(2, 0, 0.25f, 0);
	vb_ship.AddData(2, 0, 0.25f, 0);
	vb_ship.AddData(2, 0, 0, -0.25f);
	vb_ship.AddData(2, -0.25f, 0, 0);

	//Hajó alsó palást
	vb_ship.AddData(3, 0, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);;
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);
	vb_ship.AddData(3, 0.75f, 1.0f);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.75f, 1.0f);
	vb_ship.AddData(3, 1.0f, 0);
	//palást másik oldala
	vb_ship.AddData(3, 0, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);;
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);
	vb_ship.AddData(3, 0.75f, 1.0f);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.75f, 1.0f);
	vb_ship.AddData(3, 1.0f, 0);
	//Fedõlap
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0, 0.5f);
	vb_ship.AddData(3, 0.25f, 1.0f);
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.75f, 1.0f);
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0.75f, 1.0f);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 1.0f, 0.5f);
	//Gúlasátor
	vb_ship.AddData(3, 0, 1.0f);
	vb_ship.AddData(3, 0.5f, 1.0f);
	vb_ship.AddData(3, 0.5f, 0);
	vb_ship.AddData(3, 0.5f, 0);
	vb_ship.AddData(3, 0.5f, 1.0f);
	vb_ship.AddData(3, 1.0f, 1.0f);
	vb_ship.AddData(3, 1.0f, 1.0f);
	vb_ship.AddData(3, 0.5f, 1.0f);
	vb_ship.AddData(3, 0.5f, 0);
	vb_ship.AddData(3, 0.5f, 0);
	vb_ship.AddData(3, 0.5f, 1.0f);
	vb_ship.AddData(3, 0, 1.0f);

	vb_ship.InitBuffers();
}

//Hajó-zászló modell adatainak inicializálása
void Fleet::InitShipFlagVb()
{
	vb_flag.AddAttribute(0, 3); //pozíció
	vb_flag.AddAttribute(1, 3); //szín
	vb_flag.AddAttribute(2, 3); //normálvektor

	int xInvert = (isAlly ? -1 : 1);

	//Oszlop
	vb_flag.AddData(0, xInvert * 0.25f, 0, 0);
	vb_flag.AddData(0, xInvert * 0.187f, 0, -0.063f);
	vb_flag.AddData(0, xInvert * 0.187f, 0, 0.063f);
	vb_flag.AddData(0, xInvert * 0.25f, 0.375f, 0);
	vb_flag.AddData(0, xInvert * 0.187f, 0.375f, -0.063f);
	vb_flag.AddData(0, xInvert * 0.187f, 0.375f, 0.063f);
	//Zászló
	vb_flag.AddData(0, xInvert * 0.25f, 0.375f, 0);
	vb_flag.AddData(0, xInvert * 0.25f, 0.25f, 0);
	vb_flag.AddData(0, xInvert * 0.375f, 0.25f, 0);
	vb_flag.AddData(0, xInvert * 0.375f, 0.375f, 0);

	//Oszlop
	vb_flag.AddData(1, 0.5f, 0.25f, 0.1f);
	vb_flag.AddData(1, 0.5f, 0.25f, 0.1f);
	vb_flag.AddData(1, 0.5f, 0.25f, 0.1f);
	vb_flag.AddData(1, 0.5f, 0.25f, 0.1f);
	vb_flag.AddData(1, 0.5f, 0.25f, 0.1f);
	vb_flag.AddData(1, 0.5f, 0.25f, 0.1f);
	//Zászló
	if (isAlly) {
		vb_flag.AddData(1, 0, 1.0f, 0);
		vb_flag.AddData(1, 0, 1.0f, 0);
		vb_flag.AddData(1, 0, 1.0f, 0);
		vb_flag.AddData(1, 0, 1.0f, 0);
	}
	else {
		vb_flag.AddData(1, 1.0f, 0, 0);
		vb_flag.AddData(1, 1.0f, 0, 0);
		vb_flag.AddData(1, 1.0f, 0, 0);
		vb_flag.AddData(1, 1.0f, 0, 0);
	}

	//Oszlop
	vb_flag.AddData(2, 0.25f, 0, 0);
	vb_flag.AddData(2, 0.187f, 0, -0.063f);
	vb_flag.AddData(2, 0.187f, 0, 0.063f);
	vb_flag.AddData(2, 0.25f, 0.375f, 0);
	vb_flag.AddData(2, 0.187f, 0.375f, -0.063f);
	vb_flag.AddData(2, 0.187f, 0.375f, 0.063f);
	//Zászló
	vb_flag.AddData(2, 0.25f, 0.375f, 0);
	vb_flag.AddData(2, 0.25f, 0.25f, 0);
	vb_flag.AddData(2, 0.375f, 0.25f, 0);
	vb_flag.AddData(2, 0.375f, 0.375f, 0);

	//Oszlop
	vb_flag.AddIndex(0, 2, 1);
	vb_flag.AddIndex(0, 5, 2);
	vb_flag.AddIndex(0, 3, 5);
	vb_flag.AddIndex(1, 3, 0);
	vb_flag.AddIndex(1, 4, 3);
	vb_flag.AddIndex(2, 4, 1);
	vb_flag.AddIndex(2, 5, 4);
	vb_flag.AddIndex(3, 4, 5);
	//Zászló
	vb_flag.AddIndex(8, 6, 7);
	vb_flag.AddIndex(8, 9, 6);

	vb_flag.InitBuffers();
}

//Visszaadja a le nem rakott hajók számát méret alapján(1x1,2x2,stb.)
const std::array<int, 4>* Fleet::getUnplacedShipCount() const
{
	return pUnplacedShipCount;
}

//Visszaadja azoknak a mezõknek a koordinátáit amin van hajó, kilõttek is akár
const std::vector<std::vector<std::pair<char, int>>> Fleet::getShipPositions() const
{
	std::vector<std::vector<std::pair<char, int>>> result;
	for (Ship* ship : ships) {
		result.push_back(std::vector<std::pair<char, int>>());
		for (PlayTile* tile : ship->getPlayTiles()) {
			if (tile) {
				result.back().push_back(tile->getPos());
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