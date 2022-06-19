#pragma once

#include "SeaTile.h"

//Egy játékmezõ a tengeren
class PlayTile : public SeaTile
{
public:
	//PlayTile();
	PlayTile(const std::pair<char, int>& pos, const glm::vec3& trans);
	~PlayTile();

	void PreProcess(const gCamera& camera, gShaderProgram& sh_program) const;
	void Draw(const gCamera& camera, gShaderProgram& sh_program) const;
	void OutlineDraw(const gCamera& camera, gShaderProgram& sh_program) const;

	void ClearPosition();

	void setState(int new_state = 3);
	void setIndex(int new_index = 0);
	void setUsed(bool used = true);
	const int getIndex() const;
	const std::pair<char, int> getPos() const;
	bool isUsed() const;
	int getIndexOffset() const;
	const glm::vec3& getStateColor() const;

private:
	enum TileState {
		HIT = 1,
		MISS,
		DEFAULT,
		WIN_PLAYER_ONE,
		WIN_PLAYER_TWO,
		BACK_OF_THE_SHIP_PLACEABLE_HERE,
		CURSOR_SELECTION,
		UNDEFINED,
	};

	void setStateColor();

	int index; //Egy játékos játékmezõi közül hányas indexû
	const int indexOffset = 100; //offsetelni az indexet,hogy ne 0-1 legyen AMÉG ALPHA színbe írom az indexet
	std::pair<char, int> position; //Koordináta (pl. A7)
	bool usedTile = false;	//Van-e hajó rajta

	TileState state = DEFAULT;
	glm::vec3 stateColor = glm::vec3(1, 1, 1); //Alap kijelölés szín
	bool isStateChanged = false; //Eltér-e az alap színtõl a mezõ állapota
	float outlineWidth = 0.2f; //Kijelölõ négyzet vastagsága
};
