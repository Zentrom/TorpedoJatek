#pragma once

#include "../Globals.hpp"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Egy játékmezõ a tengeren
class PlayTile
{
public:
	PlayTile(const std::pair<char, int>& pos, const glm::vec3& trans);
	~PlayTile();

	void PreProcess(const gCamera& camera, gShaderProgram& sh_program, const gVertexBuffer& vb_buffer) const;
	void Draw(const gCamera& camera, gShaderProgram& sh_program, const gVertexBuffer& vb_buffer) const;
	void OutlineDraw(const gCamera& camera, gShaderProgram& sh_program, const gVertexBuffer& vb_buffer) const;

	void ClearPosition();

	void setState(int new_state = 3);
	void setIndex(int new_index = 0);
	void setUsed(bool used = true);
	const int getIndex() const;
	const std::pair<char, int> getPos() const;
	bool isUsed() const;
	int getIndexOffset() const;
	//const glm::vec3& getStateColor() const;
	const glm::vec3& getTranslate() const;
	static float getScaleXZ();

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

	const float outlineWidth = 0.2f; //Kijelölõ négyzet vastagsága
	const int indexOffset = 100; //offsetelni az indexet,hogy ne 0-1 legyen AMÉG ALPHA színbe írom az indexet

	glm::vec3 ptScale = glm::vec3(scaleXZ, 1.0f, scaleXZ) * TorpedoGLOBAL::Scale;

	int index; //Egy játékos játékmezõi közül hányas indexû
	std::pair<char, int> position; //Koordináta (pl. A7)
	bool usedTile = false;	//Van-e hajó rajta
	TileState state = DEFAULT; //Játékmezõ állapota
	glm::vec3 stateColor = glm::vec3(1, 1, 1); //Alap kijelölés szín
	bool isStateChanged = false; //Eltér-e az alap színtõl a mezõ állapota
	
	static const float scaleXZ; //tengermezõ skálázása XZ tengelyek mentén
	glm::vec3 ptTranslate = glm::vec3(0);
	const glm::mat4 matWorld; //Játékmezõ körvonallal együtti világ transzformáció
	const glm::mat4 matWorldIT; //VT inverze
	const glm::mat4 matWorldPlayTile; //Kissebb játékmezõ, hogy látszódjon a körvonal
	const glm::mat4 matWorldPlayTileIT; //PlayTileVT inverze
};
