#include "Fleet.h"

#include <iostream>

Fleet::Fleet(void)
{
}

Fleet::~Fleet(void)
{
}

void Fleet::Init(int inMapSize,bool ally) 
{
	mapSize = inMapSize;
	isAlly = ally;

	switch (mapSize) {
	case 5:
		unplacedShipCount = shipCountFive;
		break;
	case 7:
		unplacedShipCount = shipCountSeven;
		break;
	case 9:
		unplacedShipCount = shipCountNine;
		break;
	}

	firstTile_battleShipOffset = 2.0f * mapSize * TorpedoGLOBAL::Scale;

	glm::vec3 battleShipOffset = glm::vec3(firstTile_battleShipOffset, 0, 0) + glm::vec3(mountaincenter_border_Xoffset + mountain_tile_offset, 0, 0);
	battleShip = Ship(true, isAlly, battleShipOffset);
}

void Fleet::InitTiles(std::vector<PlayTile> &tiles)
{
	playTiles = tiles;
}

//void Fleet::Init(std::pair<char,int>* actPlayTiles)
//{
//	float tile_transX = 0;
//	float tile_transZ = 0;
//	glm::vec3 tileResult = glm::vec3(0.0f);
//	for (int i = 0; i < 16; i++) {
//
//		tile_transX = (actPlayTiles[i].second - 1) * 2.0f * TorpedoGLOBAL::Scale;
//		tile_transZ = ((actPlayTiles[i].first - 'a') * 2.0f * TorpedoGLOBAL::Scale) - (3.0f*2.0f* TorpedoGLOBAL::Scale);
//
//		tileResult += glm::vec3(mountaincenter_border_Xoffset, 0, 0) +
//			glm::vec3(mountain_tile_offset, 0, 0) +
//			glm::vec3(tile_transX, 0, tile_transZ);
//
//		myShips[i] = Ship(glm::vec3(-1, 1, 1)*tileResult);
//		//enemyShips[i] = Ship(tileResult);
//
//		//enemyShips[i].Init();
//		//myShips[i].Init();
//		tileResult = glm::vec3(0.0f);
//	}
//
//	glm::vec3 battleShipOffset = glm::vec3(firstTile_battleShipOffset, 0, 0) + glm::vec3(mountaincenter_border_Xoffset + mountain_tile_offset, 0, 0);
//	myBattleShip = Ship(-battleShipOffset);
//	//myBattleShip.Init();
//	enemyBattleShip = Ship(battleShipOffset);
//
//	myBattleShip.Init(false);
//	enemyBattleShip.Init(true);
//	for (int i = 0; i < 16; i++) {
//		myShips[i].Init(false);
//		//enemyShips[i].Init(true);
//	}
//}

//void Fleet::Init(std::array<int, 4> shipCount)
//{
//	unplacedShipCount = shipCount;
//}

void Fleet::Draw(gCamera &camera, gShaderProgram &sh_program)
{

	for (int i = 0; i < ships.size(); i++) {
		ships[i].Draw(camera,sh_program);
		//enemyShips[i].Draw(m_camera,m_program);
	}
	battleShip.Draw(camera, sh_program);
	//enemyBattleShip.Draw(camera,sh_program);

}

//Megnezi hogy szabad-e a tile
bool Fleet::CheckTile(PlayTile tile)
{
	//std::cout << "Checking tile: " << tile.getPos().first << tile.getPos().second << std::endl;
	for (PlayTile playTile : playTiles) {
		//std::cout << "checking tile: " << playTile.getIndex() << ' ' << playTile.getPos().first
		//	<< playTile.getPos().second << ' ' << (tile.getPos() == playTile.getPos()) 
		//	<< ' ' << playTile.isUsed() << std::endl;
		if (tile.getPos() == playTile.getPos() && playTile.isUsed()) {
			return false;
		}
	}
	return true;
}

PlayTile& Fleet::getTile(std::pair<char, int> pos)
{
	for (PlayTile &playTile : playTiles) {
		if (pos == playTile.getPos()) {
			return playTile;
		}
	}
}

std::array<PlayTile*, 4> Fleet::getFreeBacks(PlayTile &tile,int backDistance)
{
	std::array<PlayTile*, 4> result;
	bool rightFree = true;
	bool leftFree = true;
	bool upFree = true;
	bool downFree = true;
	//for (int i = 0; i < (mapSize*mapSize);i++) {
		//if (tile == playTiles[i].getPos()) {
			
	if (((tile.getIndex()%mapSize) + backDistance) >= mapSize) {
		rightFree = false;
	}
	if (((tile.getIndex()%mapSize) - backDistance) < 0) {
		leftFree = false;
	}
	if (((tile.getIndex()/mapSize) + backDistance) >= mapSize) {
		downFree = false;
	}
	if (((tile.getIndex()/mapSize) - backDistance) < 0) {
		upFree = false;
	}

	//PlayTile &tmpTile = playTiles[i];
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
			//break;
		//}
	//}

	if (rightFree) {
		result[0] = &playTiles.at(tile.getIndex() + backDistance);//std::pair<char, int>(tile.first, tile.second + backDistance);
	}
	else {
		result[0] = nullptr;
	}
	if (leftFree) {
		result[1] = &playTiles.at(tile.getIndex() - backDistance);//std::pair<char, int>(tile.first, tile.second - backDistance);
	}
	else {
		result[1] = nullptr;
	}
	if (downFree) {
		result[2] = &playTiles.at(tile.getIndex() + backDistance*mapSize);//std::pair<char, int>(static_cast<char>(tile.first+backDistance), tile.second);
	}
	else {
		result[2] = nullptr;
	}
	if (upFree) {
		result[3] = &playTiles.at(tile.getIndex() - backDistance*mapSize);//std::pair<char, int>(static_cast<char>(tile.first-backDistance), tile.second);
	}
	else {
		result[3] = nullptr;
	}

	return result;
}

void Fleet::PlaceShip(PlayTile *front, PlayTile *back)
{
	std::vector<PlayTile*> shipTiles;
	//shipTiles.push_back(front);

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

	ships.push_back(Ship(shipTiles,isAlly));
}

std::array<int, 4>& Fleet::getUnplacedShipCount()
{
	return unplacedShipCount;
}

std::vector<std::pair<char, int>> Fleet::getActiveTilePositions()
{
	std::vector<std::pair<char, int>> result;

	for (Ship ship : ships) {
		for (PlayTile* tile : ship.getPlayTiles()) {
			result.push_back(tile->getPos());
		}
	}

	return result;
}
