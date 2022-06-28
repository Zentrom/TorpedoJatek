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
	Fleet();
	~Fleet();

	void Init(int in_map_size = 7, bool ally = true);
	void InitTiles(std::vector<PlayTile> &tiles);
	void Update(float delta_time);
	void Draw(const gCamera& camera, gShaderProgram& sh_program) const;

	bool CheckTile(const PlayTile &tile) const;
	std::array<PlayTile*, 4> getFreeBacks(const PlayTile &tile, int back_distance) const;
	void PlaceShip(PlayTile *front, PlayTile *back);
	void HitFleet(std::pair<char, int> hit_pos);

	PlayTile& getTile(const std::pair<char, int> &pos);
	const std::array<int, 4>& getUnplacedShipCount() const;
	std::vector<std::pair<char, int>> getActiveTilePositions() const;
	BattleShip& getBattleShip();
private:
	std::array<int, 4> unplacedShipCount;	//1x1,2x2,stb. méretû hajókból még hányat NEM raktunk le
	std::vector<Ship*> ships;	//Játékos hajói
	std::vector<PlayTile> *playTiles; //Játékos oldalán lévõ játékmezõk
	BattleShip* battleShip;	//Játékos hajója amit nem lehet kilõni,hanem ez lõ majd.

	const float mountainTileOffset = (2.0f * PlayTile::getScaleXZ())* TorpedoGLOBAL::Scale; //Hegy és elsõ mezõ közti táv
	const float mountainCenterBorderXOffset = Mountain::getWidthX() / 2.0f * TorpedoGLOBAL::Scale; //Hegy közepe és széle közti táv
	float firstTileBattleShipOffset = PlayTile::getScaleXZ() * mapSize * TorpedoGLOBAL::Scale;	//Csatahajó és elsõ játékmezõ közti táv

	bool isAlly = true;	//Kliens szemszögébõl eza mi Fleet-ünke
	int mapSize; //Szervertõl átjött pályaméret

};
