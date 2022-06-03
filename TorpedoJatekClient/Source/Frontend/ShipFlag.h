#pragma once

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

//Haj� z�szl� oszt�lya
class ShipFlag
{
public:
	ShipFlag(void);
	ShipFlag(bool ally);
	~ShipFlag(void);

	void Draw();

private:
	void Init();

	gVertexBuffer	vb_flag; //Haj� z�szl�j�nak modell adatai

	bool isAlly = false;
};