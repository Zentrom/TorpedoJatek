#include "BShipProjectile.h"

BShipProjectile::BShipProjectile(void)
{
	Init();
}

BShipProjectile::BShipProjectile(const glm::mat4 &cannonsharedtrans, bool ally) 
	: cannonSharedTrans(cannonsharedtrans), isAlly(ally)
{
	projectileStartPos = glm::vec3(cannonSharedTrans[3][0], cannonSharedTrans[3][1], cannonSharedTrans[3][2]);
	projectileCurrentPos = projectileStartPos;
	Init();
}

BShipProjectile::~BShipProjectile(void)
{
}

//L�ved�k modell adatainak inicializ�l�sa
void BShipProjectile::Init()
{
	vb_projectile.AddAttribute(0, 3); //pos
	vb_projectile.AddAttribute(1, 3); //color
	vb_projectile.AddAttribute(2, 3); //normal

	for (int i = 0; i <= circleResHor; i++) {
		for (int j = 0; j <= circleResVert; j++) {
			float u = i / (float)circleResHor;
			float v = j / (float)circleResVert;

			//Parametrikus koordin�t�kat konvert�ljuk
			vb_projectile.AddData(0, GetUV(u, v));
			//sz�n
			vb_projectile.AddData(1, 1.0f, 0.5f, 0);
			//norm�lvektor
			vb_projectile.AddData(2, GetUV(u, v));
		}
	}
	//Hor*Vert db n�gyzet
	for (int i = 0; i < circleResHor; i++) {
		for (int j = 0; j < circleResVert; j++) {
			//n�gyzet egyik fele
			vb_projectile.AddIndex(i + j * (circleResHor + 1),
				i + (j + 1) * (circleResHor + 1),
				(i + 1) +  j * (circleResHor + 1));
			//n�gyzet m�sik fele
			vb_projectile.AddIndex((i+1) + j * (circleResHor + 1),
				i + (j + 1) * (circleResHor + 1),
				(i + 1) + (j + 1) *	(circleResHor + 1));
		}
	}

	vb_projectile.InitBuffers();
}

//Anim�ci�hoz sz�ks�ges el�k�sz�t�sek
void BShipProjectile::Fire(glm::vec3 shottilepos)
{
	projectileTargetPos = shottilepos;
	dist.x = glm::distance<float>(projectileCurrentPos.x, projectileTargetPos.x);
	dist.y = dist.x / 4.0f;
	dist.z = glm::distance<float>(projectileCurrentPos.z, projectileTargetPos.z);

	dist.x *= (isAlly ? 1.0f : -1.0f);
	dist.z *= (isAlly ? 1.0f : -1.0f);
}

//Anim�ci� - igazat ad vissza ha m�g tart
bool BShipProjectile::Animate(float deltatime)
{
	elapsedTime += deltatime;
	if (elapsedTime < animationTime) {
		float angle = elapsedTime / animationTime *  glm::pi<float>();
		float fraction = elapsedTime / animationTime;

		projectileCurrentPos += glm::vec3(fraction * dist.x, sinf(angle) * dist.y, fraction * dist.z);
		return true;
	}
	else {
		elapsedTime = 0.0f;
		projectileCurrentPos = projectileStartPos;
	}
	return false;
}

//L�ved�k kirajzol�sa
void BShipProjectile::Draw(gCamera &camera, gShaderProgram &sh_program)
{
	glm::mat4 matWorld = glm::translate(projectileCurrentPos) * glm::scale(projectileScale);
	projectileCurrentPos = projectileStartPos;

	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	sh_program.SetUniform("eye_pos", camera.GetEye());

	vb_projectile.On();
	vb_projectile.DrawIndexed(GL_TRIANGLES, 0, circleResHor * circleResVert * 2 * 3, 0);
	vb_projectile.Off();
}

//G�mb parametrikus le�r�sa: (u,v) -> (x,y,z)
glm::vec3 BShipProjectile::GetUV(float u, float v) {
	u *= 2 * glm::pi<float>();
	v *= glm::pi<float>();

	float cu = cosf(u);
	float su = sinf(u); 
	float cv = cosf(v);
	float sv = sinf(v);

	return glm::vec3(cu*sv, cv, su*sv);
}