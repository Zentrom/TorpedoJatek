#pragma once

#include "SeaTile.h"

//Egy játékmezõ a tengeren
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

	int index; //Egy játékos játékmezõi közül hányas indexû
	int indexOffset = 100; //offsetelni az indexet,hogy ne 0-1 legyen AMÉG ALPHA színbe írom az indexet
	std::pair<char, int> position;	//Koordináta (pl. A7)
	bool usedTile = false;	//Van-e hajó rajta
	glm::vec3 tileState = glm::vec3(1, 1, 1); //Alap kijelölés szín
	bool isStateChanged = false; //Eltér-e az alap színtõl a mezõ állapota
	float outlineWidth = 0.2f; //Kijelölõ négyzet vastagsága
};
