#pragma once

#include "SeaTile.h"

//Egy j�t�kmez� a tengeren
class PlayTile : public SeaTile
{
public:
	PlayTile(void);
	PlayTile(const std::pair<char, int> &pos);
	~PlayTile(void);

	void PreProcess(gCamera &camera, gShaderProgram &sh_program);
	void Draw(gCamera &camera, gShaderProgram &sh_program, GLuint &textureID);
	void OutlineDraw(gCamera &camera, gShaderProgram &sh_program);
	void setState(int newState = 3);
	void setIndex(int newIndex = 0);
	void setUsed(bool used = true);
	void ClearPosition();

	int getIndex() const;
	std::pair<char, int> getPos() const;
	bool isUsed() const;
	glm::vec3 getTranslate() const;
	int getIndexOffset() const;
	glm::vec3 getState() const;

private:

	int index; //Egy j�t�kos j�t�kmez�i k�z�l h�nyas index�
	int indexOffset = 100; //offsetelni az indexet,hogy ne 0-1 legyen AM�G ALPHA sz�nbe �rom az indexet
	std::pair<char, int> position;	//Koordin�ta (pl. A7)
	bool usedTile = false;	//Van-e haj� rajta
	glm::vec3 tileState = glm::vec3(1, 1, 1); //Alap kijel�l�s sz�n
	bool isStateChanged = false; //Elt�r-e az alap sz�nt�l a mez� �llapota
	float outlineWidth = 0.2f; //Kijel�l� n�gyzet vastags�ga
};
