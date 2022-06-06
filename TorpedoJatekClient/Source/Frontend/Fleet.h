#pragma once

#include "Ship.h"
#include "BattleShip.h"
#include "Mountain.h"
#include "../../../CommonSource/CommonGlobals.hpp"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <array>

//Egy játékos hajóit és más adatait tartalmazó osztály
class Fleet
{
public:
	Fleet(void);
	~Fleet(void);

	void Init(int inMapSize = 7, bool ally = true);
	void InitTiles(std::vector<PlayTile> &tiles);
	void Update(float deltatime);
	void Draw(gCamera &camera, gShaderProgram &sh_program);

	bool CheckTile(const PlayTile &tile);
	PlayTile& getTile(const std::pair<char, int> &pos);
	std::array<PlayTile*, 4> getFreeBacks(const PlayTile &tile, int backDistance);
	void PlaceShip(PlayTile *front, PlayTile *back);
	void HitFleet(std::pair<char, int> hitPos);

	std::array<int, 4>& getUnplacedShipCount();
	std::vector<std::pair<char, int>> getActiveTilePositions();
	BattleShip& getBattleShip();
private:
	std::array<int, 4> unplacedShipCount;	//1x1,2x2,stb. méretû hajókból még hányat NEM raktunk le

	std::vector<Ship> ships;	//Játékos hajói
	std::vector<PlayTile> *playTiles; //Játékos oldalán lévõ játékmezõk
	BattleShip battleShip;	//Játékos hajója amit nem lehet kilõni,hanem ez lõ majd.

	bool isAlly = true;	//Kliens szemszögébõl eza mi Fleet-ünke
	int mapSize;	//Szervertõl átjött pályaméret

	float firstTile_battleShipOffset = SeaTile::getScaleXZ() * mapSize * TorpedoGLOBAL::Scale;	//Csatahajó és elsõ játékmezõ közti táv
	const float mountain_tile_offset = (2.0f * SeaTile::getScaleXZ())* TorpedoGLOBAL::Scale; //Hegy és elsõ mezõ közti táv
	const float mountaincenter_border_Xoffset = Mountain::getWidth() / 2.0f * TorpedoGLOBAL::Scale; //Hegy közepe és széle közti táv

};
