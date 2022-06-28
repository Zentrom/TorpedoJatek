#pragma once

#include "../../Utility/gVertexBuffer.h"

//Haj� z�szl� oszt�lya
class ShipFlag
{
public:
	ShipFlag(bool ally);
	~ShipFlag();

	void Draw() const;

private:
	void Init();

	gVertexBuffer vb_flag; //Haj� z�szl�j�nak modell adatai

	const bool isAlly = false; //J�t�kos vagy ellenf�l z�szl�ja
};