#include "BShipCannon.h"

BShipCannon::BShipCannon(void)
{
	Init();
}

BShipCannon::BShipCannon(const glm::vec3 &battleShipTranslate,bool ally, float battleShipScaleX) 
	: bShipTranslate(battleShipTranslate)
{
	alliedRotation = (ally ? glm::pi<float>() : 0.0f);
	cannonTranslateX = battleShipScaleX / 8.0f;
	Init();

	projectileSharedTrans = glm::translate(bShipTranslate) * glm::rotate(alliedRotation, glm::vec3(0, 1.0f, 0))
		* glm::translate(glm::vec3(-cannonTranslateX, 0.25f, 0));
	projectile = new BShipProjectile(projectileSharedTrans, ally);
}

BShipCannon::~BShipCannon(void)
{
}

//Ágyú modell adatainak inicializálása
void BShipCannon::Init()
{
	vb_cannon.AddAttribute(0, 3); //pozíció
	vb_cannon.AddAttribute(1, 3); //szín
	vb_cannon.AddAttribute(2, 3); //normálvektor
	vb_cannon.AddAttribute(3, 2); //textúrakoord

	//Palást
	float height = 1.0f;
	for (int i = 0; i <= circleRes; i++) {

		float angle = i * 2*glm::pi<float>() / circleRes; // szög

		vb_cannon.AddData(0, cosf(angle), 0, sinf(angle));
		vb_cannon.AddData(0, cosf(angle), height, sinf(angle));

		//szín
		vb_cannon.AddData(1, 0.1f, 0.1f, 0.1f);
		vb_cannon.AddData(1, 0.1f, 0.1f, 0.1f);

		glm::vec3 normal = glm::normalize(glm::vec3(cosf(angle), 0, sinf(angle)));
		vb_cannon.AddData(2, normal.x, 0, normal.z);
		vb_cannon.AddData(2, normal.x, 0, normal.z);

		vb_cannon.AddData(3, 2 * (float)i / circleRes, 1);
		vb_cannon.AddData(3, 2 * (float)i / circleRes, 0);
	}

	for (int i = 0; i < circleRes; i++) {
		vb_cannon.AddIndex(2 * i, 2 * i + 1, 2 * i + 2);
		vb_cannon.AddIndex(2 * i + 1, 2 * i + 3, 2 * i + 2);
	}

	//Tetõ
	vb_cannon.AddData(0, 0, height, 0);
	vb_cannon.AddData(1, 0.1f, 0.1f, 0.1f);
	vb_cannon.AddData(2, 0, 1, 0);
	vb_cannon.AddData(3, 0.5, 0.5);

	for (int i = 0; i <= circleRes; i++) {

		float angle = i * 2 * glm::pi<float>() / circleRes;
		vb_cannon.AddData(0, cosf(angle), height, sinf(angle));
		vb_cannon.AddData(1, 0.1f, 0.1f, 0.1f);
		
		vb_cannon.AddData(2, 0, 1, 0);
		
		vb_cannon.AddData(3, 0.5f * cosf(angle) + 0.5f, 0.5f * sinf(angle) + 0.5);
	}
	
	for (int i = circleRes*2+3; i <= circleRes*3; i++) {
		vb_cannon.AddIndex(circleRes*2+3, i + 2, i + 1);
	}

	vb_cannon.InitBuffers();

	cannonTextureID = GLUtils::TextureFromFile("Resources/Textures/cannonTexture.bmp");
}

//Ágyú kirajzolása
void BShipCannon::Draw(gCamera& camera, gShaderProgram& sh_program, glm::mat4 sharedtrans)
{
	glm::mat4 matWorld = projectileSharedTrans * sharedtrans * glm::rotate(cannonRotate, cannonRotateAngle)
		* glm::scale(cannonScale); 
	
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	sh_program.SetUniform("eye_pos", camera.GetEye());

	sh_program.SetUniform("hasTexture", true);
	sh_program.SetTexture("texImage", 0, cannonTextureID);

	vb_cannon.On();
	vb_cannon.DrawIndexed(GL_TRIANGLES, 0, circleRes * 3 * 3, 0);
	vb_cannon.Off();
	sh_program.SetUniform("hasTexture", false);

	sh_program.Off();
	projectile->Draw(camera, sharedtrans);
	sh_program.On();
}

BShipProjectile& BShipCannon::getProjectile()
{
	return *projectile;
}
