#pragma once

#include "../../Utility/gVertexBuffer.h"

//Hajó zászló osztálya
class ShipFlag
{
public:
	ShipFlag(bool ally);
	~ShipFlag();

	void Draw() const;

private:
	void Init();

	gVertexBuffer vb_flag; //Hajó zászlójának modell adatai

	const bool isAlly = false; //Játékos vagy ellenfél zászlója
};