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
	PlayTile(const std::pair<char, int>& pos, const glm::vec3& trans, const glm::vec3& scale, int ind, int ident_num);
	~PlayTile();

	void PreProcess(const gCamera& camera, gShaderProgram& sh_program, const gVertexBuffer& vb_buffer) const;
	void Draw(const gCamera& camera, gShaderProgram& sh_program, const gVertexBuffer& vb_buffer) const;
	void OutlineDraw(const gCamera& camera, gShaderProgram& sh_program, const gVertexBuffer& vb_buffer) const;

	void ClearPosition();

	void setState(int new_state = 3);
	void setUsed(bool used = true);
	const int getId() const;
	const int getIndex() const;
	const std::pair<char, int> getPos() const;
	bool isUsed() const;
	const glm::vec3& getTranslate() const;
	const int getState() const;

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

	const float outlineWidth = 0.2f * TorpedoGLOBAL::Scale; //Kijelölõ négyzet vastagsága

	bool isStateChanged = false; //Eltér-e az alap színtõl a mezõ állapota
	bool usedTile = false;	//Van-e hajó rajta
	TileState state = DEFAULT; //Játékmezõ állapota
	glm::vec3 stateColor = glm::vec3(1, 1, 1); //Alap kijelölés szín
	
	std::pair<char, int> position; //Koordináta (pl. A7)
	const int index; //std::vector<PlayTiles*>-ba hányadik indexü
	const int id; //Játékmezõ azonosítója a 3D pickinghez
	const glm::vec3 ptTranslate; //Mozgatás
	const glm::vec3 ptScale; //Skálázás
	const glm::mat4 matWorldOutline; //Játékmezõ körvonallal együtti világ transzformáció
	const glm::mat4 matWorldPlayTile; //Kissebb játékmezõ, hogy látszódjon a körvonal
	const glm::mat4 matWorldPlayTileIT; //PlayTileVT inverze
};
