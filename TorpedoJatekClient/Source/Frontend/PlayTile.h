#pragma once

#include "SeaTile.h"

//Egy játékmezõ a tengeren
class PlayTile : public SeaTile
{
public:
	PlayTile(void);
	PlayTile(const std::pair<char, int> &pos);
	~PlayTile(void);

	void Draw(gCamera &camera, gShaderProgram &sh_playtile);
	void setState(int newState = 3);
	void setIndex(int newIndex = 0);
	void setUsed(bool used = true);
	void ClearPosition();

	int getIndex() const;
	std::pair<char, int> getPos() const;
	bool isUsed() const;
	glm::vec3 getTranslate() const;

private:

	int index; //Egy játékos játékmezõi közül hányas indexû
	int indexOffset = 100; //offsetelni az indexet,hogy ne 0-1 legyen 
	std::pair<char, int> position;	//Koordináta (pl. A7)
	bool usedTile = false;	//Van-e hajó rajta
	glm::vec3 tileState = glm::vec3(0, 0, 1); //3kek,2sarga,1piros-sárga=nemtalált,piros=talált

};
