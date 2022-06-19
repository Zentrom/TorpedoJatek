#pragma once

#include "../Globals.hpp"
#include "BShipProjectile.h"

#include "../../Utility/gVertexBuffer.h"
#include "../../Utility/gShaderProgram.h"
#include "../../Utility/gCamera.h"
#include "../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Csatahaj� �gy�ja
class BShipCannon
{
public:
	BShipCannon(void);
	BShipCannon(const glm::vec3 &battleShipTranslate,bool ally, float battleShipScaleX);
	~BShipCannon(void);

	void Draw(gCamera& camera, gShaderProgram& sh_program, glm::mat4 sharedtrans);
	BShipProjectile& getProjectile();
private:
	void Init();

	BShipProjectile* projectile; //L�ved�k pointer
	glm::mat4 projectileSharedTrans; //L�ved�kkel azonos transzform�c�k

	int circleRes = 16; //k�rfelbont�s

	gVertexBuffer	vb_cannon; //�gy� grafikai modell adatai
	GLuint cannonTextureID; //�gy� text�ra azonos�t�ja

	float alliedRotation = 0.0f; //forgat�si �rt�k ha a mi �gyunk
	glm::vec3 bShipTranslate; //csatahaj� elmozgat�sa
	float cannonTranslateX; //mozgat�s
	float cannonRotate = 0.25f * glm::pi<float>(); //forgat�s
	glm::vec3 cannonRotateAngle = glm::vec3(0, 0, 1.0f); //forgat�s tengelye
	glm::vec3 cannonScale = glm::vec3(0.125f, 0.5f, 0.125f) * TorpedoGLOBAL::Scale; //sk�l�z�s
};