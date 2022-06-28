#include "ShipFlag.h"

ShipFlag::ShipFlag(bool ally) : isAlly(ally)
{
	Init();
}

ShipFlag::~ShipFlag(void)
{
	vb_flag.Clean();
}

//Haj� z�szl� modell adatainak inicializ�l�sa
void ShipFlag::Init()
{
	vb_flag.AddAttribute(0, 3); //poz�ci�
	vb_flag.AddAttribute(1, 3); //sz�n
	vb_flag.AddAttribute(2, 3); //norm�lvektor
	
	int xInvert = (isAlly ? -1 : 1);
	
	//Oszlop
	vb_flag.AddData(0, xInvert*0.25f, 0, 0);
	vb_flag.AddData(0, xInvert*0.187f, 0, -0.063f);
	vb_flag.AddData(0, xInvert*0.187f, 0, 0.063f);
	vb_flag.AddData(0, xInvert*0.25f, 0.375f, 0);
	vb_flag.AddData(0, xInvert*0.187f, 0.375f, -0.063f);
	vb_flag.AddData(0, xInvert*0.187f, 0.375f, 0.063f);
	//Z�szl�
	vb_flag.AddData(0, xInvert*0.25f, 0.375f, 0);
	vb_flag.AddData(0, xInvert*0.25f, 0.25f, 0);
	vb_flag.AddData(0, xInvert*0.375f, 0.25f, 0);
	vb_flag.AddData(0, xInvert*0.375f, 0.375f, 0);
	
	//Oszlop
	vb_flag.AddData(1, 0.5f, 0.25f, 0.1f);
	vb_flag.AddData(1, 0.5f, 0.25f, 0.1f);
	vb_flag.AddData(1, 0.5f, 0.25f, 0.1f);
	vb_flag.AddData(1, 0.5f, 0.25f, 0.1f);
	vb_flag.AddData(1, 0.5f, 0.25f, 0.1f);
	vb_flag.AddData(1, 0.5f, 0.25f, 0.1f);
	//Z�szl�
	if (isAlly) {
		vb_flag.AddData(1, 0, 1.0f, 0);
		vb_flag.AddData(1, 0, 1.0f, 0);
		vb_flag.AddData(1, 0, 1.0f, 0);
		vb_flag.AddData(1, 0, 1.0f, 0);
	}
	else {
		vb_flag.AddData(1, 1.0f, 0, 0);
		vb_flag.AddData(1, 1.0f, 0, 0);
		vb_flag.AddData(1, 1.0f, 0, 0);
		vb_flag.AddData(1, 1.0f, 0, 0);
	}
	
	//Oszlop
	vb_flag.AddData(2, 0.25f, 0, 0);
	vb_flag.AddData(2, 0.187f, 0, -0.063f);
	vb_flag.AddData(2, 0.187f, 0, 0.063f);
	vb_flag.AddData(2, 0.25f, 0.375f, 0);
	vb_flag.AddData(2, 0.187f, 0.375f, -0.063f);
	vb_flag.AddData(2, 0.187f, 0.375f, 0.063f);
	//Z�szl�
	vb_flag.AddData(2, 0.25f, 0.375f, 0);
	vb_flag.AddData(2, 0.25f, 0.25f, 0);
	vb_flag.AddData(2, 0.375f, 0.25f, 0);
	vb_flag.AddData(2, 0.375f, 0.375f, 0);
	
	//Oszlop
	vb_flag.AddIndex(0, 2, 1);
	vb_flag.AddIndex(0, 5, 2);
	vb_flag.AddIndex(0, 3, 5);
	vb_flag.AddIndex(1, 3, 0);
	vb_flag.AddIndex(1, 4, 3);
	vb_flag.AddIndex(2, 4, 1);
	vb_flag.AddIndex(2, 5, 4);
	vb_flag.AddIndex(3, 4, 5);
	//Z�szl�
	vb_flag.AddIndex(8, 6, 7);
	vb_flag.AddIndex(8, 9, 6);
	
	vb_flag.InitBuffers();
}

//Egy haj� z�szl�j�nak kirajzol�sa
void ShipFlag::Draw() const
{
	vb_flag.On();
	glDisable(GL_CULL_FACE);
	vb_flag.DrawIndexed(GL_TRIANGLES, 0, 30, 0);
	glEnable(GL_CULL_FACE);
	vb_flag.Off();
}
