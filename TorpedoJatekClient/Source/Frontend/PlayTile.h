#pragma once

#include "SeaTile.h"

class PlayTile : public SeaTile
{
public:
	PlayTile(void);
	//PlayTile(glm::vec3 translate);
	PlayTile(std::pair<char, int> pos);
	~PlayTile(void);

	void Draw(gCamera &camera,gShaderProgram &sh_playtile);
	void setState(int newState);
	void setIndex(int newIndex);
	void setUsed(bool used);

	int getIndex();
	std::pair<char, int> getPos();
	bool isUsed();

	glm::vec3 getTranslate();

private:
	
	int index;
	std::pair<char, int> position;
	bool usedTile = false;
	glm::vec3 tileState = glm::vec3(0, 0, 1); //3kek,2sarga,1piros

};
