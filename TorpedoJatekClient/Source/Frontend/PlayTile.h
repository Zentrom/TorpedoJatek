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

	const float outlineWidth = 0.2f; //Kijel�l� n�gyzet vastags�ga
	const int indexOffset = 100; //offsetelni az indexet,hogy ne 0-1 legyen AM�G ALPHA sz�nbe �rom az indexet

	glm::vec3 ptScale = glm::vec3(scaleXZ, 1.0f, scaleXZ) * TorpedoGLOBAL::Scale;

	int index; //Egy j�t�kos j�t�kmez�i k�z�l h�nyas index�
	std::pair<char, int> position; //Koordin�ta (pl. A7)
	bool usedTile = false;	//Van-e haj� rajta
	TileState state = DEFAULT; //J�t�kmez� �llapota
	glm::vec3 stateColor = glm::vec3(1, 1, 1); //Alap kijel�l�s sz�n
	bool isStateChanged = false; //Elt�r-e az alap sz�nt�l a mez� �llapota
	
	static const float scaleXZ; //tengermez� sk�l�z�sa XZ tengelyek ment�n
	glm::vec3 ptTranslate = glm::vec3(0);
	const glm::mat4 matWorld; //J�t�kmez� k�rvonallal egy�tti vil�g transzform�ci�
	const glm::mat4 matWorldIT; //VT inverze
	const glm::mat4 matWorldPlayTile; //Kissebb j�t�kmez�, hogy l�tsz�djon a k�rvonal
	const glm::mat4 matWorldPlayTileIT; //PlayTileVT inverze
};
