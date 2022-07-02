#pragma once

#include "../Globals.hpp"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Egy j�t�kmez� a tengeren
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

	const float outlineWidth = 0.2f * TorpedoGLOBAL::Scale; //Kijel�l� n�gyzet vastags�ga

	bool isStateChanged = false; //Elt�r-e az alap sz�nt�l a mez� �llapota
	bool usedTile = false;	//Van-e haj� rajta
	TileState state = DEFAULT; //J�t�kmez� �llapota
	glm::vec3 stateColor = glm::vec3(1, 1, 1); //Alap kijel�l�s sz�n
	
	std::pair<char, int> position; //Koordin�ta (pl. A7)
	const int index; //std::vector<PlayTiles*>-ba h�nyadik index�
	const int id; //J�t�kmez� azonos�t�ja a 3D pickinghez
	const glm::vec3 ptTranslate; //Mozgat�s
	const glm::vec3 ptScale; //Sk�l�z�s
	const glm::mat4 matWorldOutline; //J�t�kmez� k�rvonallal egy�tti vil�g transzform�ci�
	const glm::mat4 matWorldPlayTile; //Kissebb j�t�kmez�, hogy l�tsz�djon a k�rvonal
	const glm::mat4 matWorldPlayTileIT; //PlayTileVT inverze
};
