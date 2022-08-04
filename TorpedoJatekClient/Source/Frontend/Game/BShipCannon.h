#pragma once

#include "../../Globals.hpp"
#include "BShipProjectile.h"

#include "../../../Utility/gVertexBuffer.h"
#include "../../../Utility/gShaderProgram.h"
#include "../../../Utility/gCamera.h"
#include "../../../Utility/GLUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//Csatahaj� �gy�ja
class BShipCannon
{
public:
	BShipCannon(const glm::vec3& battle_ship_translate,bool ally, float battle_ship_scale_x);
	~BShipCannon();

	void Draw(const gCamera& camera, gShaderProgram& sh_program, const glm::mat4& shared_trans) const;
	BShipProjectile& getProjectile() const;
private:
	void Init();

	BShipProjectile* projectile; //L�ved�k pointer
	gVertexBuffer	vb_cannon; //�gy� grafikai modell adatai
	GLuint cannonTextureID; //�gy� text�ra azonos�t�ja

	const int circleRes = 16; //k�rfelbont�s
	const float cannonBShipDivFactor = 8.0f; //�gy� mozgat�sa haj� ter�let�n bel�l(oszt�s)
	const float cannonTranslateY = 0.25f; //Mozgat�s Y tengelyen

	const float cannonRotate = 0.25f * glm::pi<float>(); //forgat�s
	const glm::vec3 cannonRotateAngle = glm::vec3(0, 0, 1.0f); //forgat�s tengelye
	const glm::vec3 cannonScale = glm::vec3(0.125f, 0.5f, 0.125f) * TorpedoGLOBAL::Scale; //sk�l�z�s

	glm::mat4 projectileSharedTrans; //L�ved�kkel azonos transzform�ci�k

	const glm::vec3 bShipTranslate; //csatahaj� elmozgat�sa
	const float alliedRotation; //forgat�si �rt�k ha a mi �gyunk
	const float cannonTranslateX; //mozgat�s X tengelyen
};