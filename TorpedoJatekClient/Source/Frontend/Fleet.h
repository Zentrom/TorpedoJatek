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

//Egy j�t�kos haj�it �s m�s adatait tartalmaz� oszt�ly
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
	std::array<int, 4> unplacedShipCount;	//1x1,2x2,stb. m�ret� haj�kb�l m�g h�nyat NEM raktunk le
	std::vector<Ship*> ships;	//J�t�kos haj�i
	std::vector<PlayTile> *playTiles; //J�t�kos oldal�n l�v� j�t�kmez�k
	BattleShip* battleShip;	//J�t�kos haj�ja amit nem lehet kil�ni,hanem ez l� majd.

	const float mountainTileOffset = (2.0f * PlayTile::getScaleXZ())* TorpedoGLOBAL::Scale; //Hegy �s els� mez� k�zti t�v
	const float mountainCenterBorderXOffset = Mountain::getWidthX() / 2.0f * TorpedoGLOBAL::Scale; //Hegy k�zepe �s sz�le k�zti t�v
	float firstTileBattleShipOffset = PlayTile::getScaleXZ() * mapSize * TorpedoGLOBAL::Scale;	//Csatahaj� �s els� j�t�kmez� k�zti t�v

	bool isAlly = true;	//Kliens szemsz�g�b�l eza mi Fleet-�nke
	int mapSize; //Szervert�l �tj�tt p�lyam�ret

};
