#pragma once

#include "SeaTile.h"

//Egy j�t�kmez� a tengeren
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

	int getIndex() const;
	std::pair<char, int> getPos() const;
	bool isUsed() const;
	glm::vec3 getTranslate() const;

private:

	int index; //Egy j�t�kos j�t�kmez�i k�z�l h�nyas index�
	std::pair<char, int> position;	//Koordin�ta (pl. A7)
	bool usedTile = false;	//Van-e haj� rajta
	glm::vec4 tileState = glm::vec4(0, 0, 1, 1); //3kek,2sarga,1piros-s�rga=nemtal�lt,piros=tal�lt

};
