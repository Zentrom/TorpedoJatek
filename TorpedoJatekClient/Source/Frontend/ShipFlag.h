#pragma once

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"

//Hajó zászló osztálya
class ShipFlag
{
public:
	ShipFlag(void);
	ShipFlag(bool ally);
	~ShipFlag(void);

	void Draw();

private:
	void Init();

	gVertexBuffer	vb_flag; //Hajó zászlójának modell adatai

	bool isAlly = false;
};