#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <array>

#include "Ship.h"

class Fleet
{
public:
	Fleet(void);
	~Fleet(void);

	//void Init(std::pair<char,int>* actPlayTiles);
	void Init(int inMapSize,bool ally);
	void InitTiles(std::vector<PlayTile> &tiles);
	//void Init(std::array<int, 4> shipCount);
	void Draw(gCamera &camera, gShaderProgram &sh_program);

	bool CheckTile(PlayTile tile);
	PlayTile& getTile(std::pair<char,int> pos);
	std::array<PlayTile*, 4> getFreeBacks(PlayTile &tile,int backDistance);
	void PlaceShip(PlayTile *front, PlayTile *back);

	std::array<int, 4>& getUnplacedShipCount();
	std::vector<std::pair<char, int>> getActiveTilePositions();
private:

	//Hajoszam a palyanagysag fuggvenyeben(1x1,2x2,stb.)
	const std::array<int, 4> shipCountFive = { 3,1,1,0 };
	const std::array<int, 4> shipCountSeven = { 4,3,2,0 };
	const std::array<int, 4> shipCountNine = { 6,4,3,1 };

	std::array<int, 4> unplacedShipCount;

	std::vector<Ship> ships;
	std::vector<PlayTile> playTiles; //csak az aktivak kéne a Fleetbe is legyenek majd szerintem.
	//Ship enemyShips[16];
	Ship battleShip;
	//Ship enemyBattleShip;
	
	bool isAlly = true;
	int mapSize;
	//std::map<int,std::vector<Ship>> ships;
	//Ship battleShip;

	int fleetTranslation;
	float firstTile_battleShipOffset = 2.0f*mapSize * TorpedoGLOBAL::Scale;
	const float mountain_tile_offset = (4.0f + 1.0f)* TorpedoGLOBAL::Scale; //tilescale fele
	const float mountaincenter_border_Xoffset = 6.0f / 2.0f * TorpedoGLOBAL::Scale; //scale2irányba megy

};
