#include "BShipCannon.h"

//BShipCannon::BShipCannon(void)
//{
//	Init();
//}

BShipCannon::BShipCannon(const glm::vec3 &battle_ship_translate,bool ally, float battle_ship_scale_x) 
	: bShipTranslate(battle_ship_translate), alliedRotation(ally ? glm::pi<float>() : 0.0f),
	cannonTranslateX(battle_ship_scale_x / cannonBShipDivFactor)
{
	//alliedRotation = (ally ? glm::pi<float>() : 0.0f);
	//cannonTranslateX = battleShipScaleX / 8.0f;
	Init();

	projectileSharedTrans = glm::translate(bShipTranslate) * glm::rotate(alliedRotation, glm::vec3(0, 1.0f, 0))
		* glm::translate(glm::vec3(-cannonTranslateX, cannonTranslateY, 0));
	projectile = new BShipProjectile(projectileSharedTrans, ally);
}

BShipCannon::~BShipCannon(void)
{
	vb_cannon.Clean();
	glDeleteTextures(1, &cannonTextureID);
	delete projectile;
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
	for (int i = 0; i <= circleRes; ++i) {

		float angle = i * 2 *glm::pi<float>() / circleRes; // szög
		//pos
		vb_cannon.AddData(0, cosf(angle), 0, sinf(angle));
		vb_cannon.AddData(0, cosf(angle), height, sinf(angle));
		//szín
		vb_cannon.AddData(1, 0.1f, 0.1f, 0.1f);
		vb_cannon.AddData(1, 0.1f, 0.1f, 0.1f);
		//normal
		glm::vec3 normal = glm::normalize(glm::vec3(cosf(angle), 0, sinf(angle)));
		vb_cannon.AddData(2, normal.x, 0, normal.z);
		vb_cannon.AddData(2, normal.x, 0, normal.z);
		//textcoord
		vb_cannon.AddData(3, 2 * i / static_cast<float>(circleRes), 0);
		vb_cannon.AddData(3, 2 * i / static_cast<float>(circleRes), 1);
	}

	for (int i = 0; i < circleRes; ++i) {
		vb_cannon.AddIndex(2 * i, 2 * i + 1, 2 * i + 2);
		vb_cannon.AddIndex(2 * i + 1, 2 * i + 3, 2 * i + 2);
	}

	//Tetõ
	vb_cannon.AddData(0, 0, height, 0);
	vb_cannon.AddData(1, 0.1f, 0.1f, 0.1f);
	vb_cannon.AddData(2, 0, 1, 0);
	vb_cannon.AddData(3, 0.5, 0.5);

	for (int i = 0; i <= circleRes; ++i) {

		float angle = i * 2 * glm::pi<float>() / circleRes;
		vb_cannon.AddData(0, cosf(angle), height, sinf(angle));
		vb_cannon.AddData(1, 0.1f, 0.1f, 0.1f);
		
		vb_cannon.AddData(2, 0, 1, 0);
		
		vb_cannon.AddData(3, 0.5f * cosf(angle) + 0.5f, 0.5f * sinf(angle) + 0.5);
	}
	
	for (int i = circleRes*2+3; i <= circleRes*3; ++i) {
		vb_cannon.AddIndex(circleRes*2+3, i + 2, i + 1);
	}

	vb_cannon.InitBuffers();

	cannonTextureID = GLUtils::TextureFromFile("Resources/Textures/cannonTexture.bmp");
}

//Ágyú kirajzolása
void BShipCannon::Draw(const gCamera& camera, gShaderProgram& sh_program, const glm::mat4& shared_trans) const
{
	glm::mat4 matWorld = projectileSharedTrans * shared_trans * glm::rotate(cannonRotate, cannonRotateAngle)
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
	projectile->Draw(camera, shared_trans);
	sh_program.On();
}

BShipProjectile& BShipCannon::getProjectile() const
{
	return *projectile;
}
