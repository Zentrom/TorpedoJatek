#pragma once

#include "BattleShip.h"
#include "../../../../CommonSource/CommonGlobals.hpp"

#include "../../../Utility/gVertexBuffer.h"
#include "../../../Utility/gShaderProgram.h"
#include "../../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <array>

//Egy j�t�kos haj�it �s m�s adatait tartalmaz� oszt�ly
class Fleet
{
public:
	Fleet(bool ally);
	~Fleet();

	void Init(int in_map_size, float pt_center_offset);
	void InitTiles(std::vector<PlayTile*> &tiles);
	void Update(float delta_time);
	void Draw(const gCamera& camera, gShaderProgram& sh_program) const;

	bool CheckTile(const PlayTile &tile) const;
	std::array<PlayTile*, 4> getFreeBacks(const PlayTile &tile, int back_distance) const;
	void PlaceShip(PlayTile *front, PlayTile *back);
	void HitFleet(const std::pair<char, int> hit_pos);

	PlayTile& getTile(const std::pair<char, int> &pos);
	const std::array<int, 4>* getUnplacedShipCount() const;
	const std::vector<std::vector<std::pair<char, int>>> getShipPositions() const;
	BattleShip& getBattleShip();
	const bool getIsAlly() const;
private:
	void InitShipVb();
	void InitShipFlagVb();

	gVertexBuffer vb_ship; //Haj� grafikai modell adatai
	gVertexBuffer vb_flag; //Haj� z�szl�j�nak modell adatai
	const std::array<int, 4> *pUnplacedShipCount; //1x1,2x2,stb. m�ret� haj�kb�l m�g h�nyat NEM raktunk le
	std::vector<Ship*> ships; //J�t�kos haj�i
	std::vector<PlayTile*> *pPlayTiles; //J�t�kos oldal�n l�v� j�t�kmez�k
	BattleShip* battleShip;	//J�t�kos haj�ja amit nem lehet kil�ni,hanem ez l� majd.

	const float bShipPlayZoneOffset = 1.0f; //Csatahaj� t�vols�ga a j�t�kt�r v�g�t�l

	int mapSize; //Szervert�l �tj�tt p�lyam�ret

	bool isAlly; //Kliens szemsz�g�b�l eza mi Fleet-�nke

};
