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

//Egy játékos hajóit és más adatait tartalmazó osztály
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

	gVertexBuffer vb_ship; //Hajó grafikai modell adatai
	gVertexBuffer vb_flag; //Hajó zászlójának modell adatai
	const std::array<int, 4> *pUnplacedShipCount; //1x1,2x2,stb. méretû hajókból még hányat NEM raktunk le
	std::vector<Ship*> ships; //Játékos hajói
	std::vector<PlayTile*> *pPlayTiles; //Játékos oldalán lévõ játékmezõk
	BattleShip* battleShip;	//Játékos hajója amit nem lehet kilõni,hanem ez lõ majd.

	const float bShipPlayZoneOffset = 1.0f; //Csatahajó távolsága a játéktér végétõl

	int mapSize; //Szervertõl átjött pályaméret

	bool isAlly; //Kliens szemszögébõl eza mi Fleet-ünke

};
