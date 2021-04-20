#pragma once

#include "Ship.h"
#include "../../../CommonSource/CommonGlobals.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <array>

//Egy j�t�kos haj�it �s m�s adatait tartalmaz� oszt�ly
class Fleet
{
public:
	Fleet(void);
	~Fleet(void);

	void Init(int inMapSize = 7, bool ally = true);
	void InitTiles(const std::vector<PlayTile> &tiles);
	void Draw(gCamera &camera, gShaderProgram &sh_program);

	bool CheckTile(const PlayTile &tile);
	PlayTile& getTile(const std::pair<char, int> &pos);
	std::array<PlayTile*, 4> getFreeBacks(const PlayTile &tile, int backDistance);
	void PlaceShip(PlayTile *front, PlayTile *back);

	std::array<int, 4>& getUnplacedShipCount();
	std::vector<std::pair<char, int>> getActiveTilePositions();
private:
	std::array<int, 4> unplacedShipCount;	//1x1,2x2,stb. m�ret� haj�kb�l m�g h�nyat NEM raktunk le

	std::vector<Ship> ships;	//J�t�kos haj�i
	std::vector<PlayTile> playTiles; //csak az aktivak k�ne a Fleetbe is legyenek majd szerintem.Most van passz�v is.
	Ship battleShip;	//J�t�kos haj�ja amit nem lehet kil�ni,hanem ez l� majd.

	bool isAlly = true;	//Kliens szemsz�g�b�l eza mi Fleet-�nke
	int mapSize;	//Szervert�l �tj�tt p�lyam�ret

	float firstTile_battleShipOffset = 2.0f*mapSize * TorpedoGLOBAL::Scale;	//Csatahaj� �s els� j�t�kmez� k�zti t�v
	const float mountain_tile_offset = (4.0f + 1.0f)* TorpedoGLOBAL::Scale; //Hegy �s els� mez� k�zti t�v
	float mountaincenter_border_Xoffset = 6.0f / 2.0f * TorpedoGLOBAL::Scale; //Hegy k�zepe �s sz�le k�zti t�v

};
