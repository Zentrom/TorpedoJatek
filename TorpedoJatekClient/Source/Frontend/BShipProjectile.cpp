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
	projectileParticle = new ParticleGroup(projectileScale.x);
	Init();
}

BShipProjectile::~BShipProjectile(void)
{
}

//Lövedék modell adatainak inicializálása
void BShipProjectile::Init()
{
	vb_projectile.AddAttribute(0, 3); //pos
	vb_projectile.AddAttribute(1, 3); //color
	vb_projectile.AddAttribute(2, 3); //normal

	for (int i = 0; i <= circleResHor; i++) {
		for (int j = 0; j <= circleResVert; j++) {
			float u = i / (float)circleResHor;
			float v = j / (float)circleResVert;

			//Parametrikus koordinátákat konvertáljuk
			vb_projectile.AddData(0, GetUV(u, v));
			//szín
			vb_projectile.AddData(1, 1.0f, 0.5f, 0);
			//normálvektor
			vb_projectile.AddData(2, GetUV(u, v));
		}
	}
	//Hor*Vert db négyzet
	for (int i = 0; i < circleResHor; i++) {
		for (int j = 0; j < circleResVert; j++) {
			//négyzet egyik fele
			vb_projectile.AddIndex(i + j * (circleResHor + 1),
				i + (j + 1) * (circleResHor + 1),
				(i + 1) +  j * (circleResHor + 1));
			//négyzet másik fele
			vb_projectile.AddIndex((i+1) + j * (circleResHor + 1),
				i + (j + 1) * (circleResHor + 1),
				(i + 1) + (j + 1) *	(circleResHor + 1));
		}
	}

	vb_projectile.InitBuffers();

	sh_projectile.AttachShader(GL_VERTEX_SHADER, "Shaders/projectile.vert");
	sh_projectile.AttachShader(GL_FRAGMENT_SHADER, "Shaders/projectile.frag");
	sh_projectile.BindAttribLoc(0, "vs_in_pos");
	sh_projectile.BindAttribLoc(1, "vs_in_col");
	sh_projectile.LinkProgram();
}

//Animációhoz szükséges elõkészítések
void BShipProjectile::Fire(glm::vec3 shottilepos)
{
	projectileTargetPos = shottilepos;
	dist.x = glm::distance<float>(projectileCurrentPos.x, projectileTargetPos.x);
	dist.y = dist.x / 4.0f;
	dist.z = projectileTargetPos.z - projectileCurrentPos.z;
	//dist.z = glm::distance<float>(projectileCurrentPos.z, projectileTargetPos.z);

	dist.x *= (isAlly ? 1.0f : -1.0f);
	//dist.z *= (isAlly ? 1.0f : -1.0f); VALSZEG NEMKELL HA EDDIG MENT
}

//Animáció - igazat ad vissza ha még tart
bool BShipProjectile::Animate(float deltatime)
{
	elapsedTime += deltatime;
	if (elapsedTime < animationTime) {
		float angle = elapsedTime / animationTime *  glm::pi<float>();
		float fraction = elapsedTime / animationTime;
		projectileCurrentPos = projectileStartPos + glm::vec3(fraction * dist.x, sinf(angle) * dist.y, fraction * dist.z);

		particleElapsed += deltatime;
		if (particleElapsed > particleEmitTime) {
			particleElapsed = 0.0f;
			particles.push_back(std::pair<glm::vec3,float>(projectileCurrentPos, particleLife));
		}
		for (std::pair<glm::vec3, float> &par : particles) {
			if (par.second > 0) {
				par.second -= deltatime;
			}
		}

		return true;
	}
	else {
		elapsedTime = 0.0f;
		particleElapsed = 0.0f;
		particles.clear();
		projectileCurrentPos = projectileStartPos;
	}
	return false;
}

//Lövedék kirajzolása
void BShipProjectile::Draw(gCamera& camera, glm::mat4 sharedtrans)
{
	glm::mat4 matWorld = glm::translate(projectileCurrentPos) * sharedtrans * glm::scale(projectileScale);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	sh_projectile.On();

	sh_projectile.SetUniform("world", matWorld);
	sh_projectile.SetUniform("worldIT", matWorldIT);
	sh_projectile.SetUniform("MVP", mvp);

	vb_projectile.On();
	vb_projectile.DrawIndexed(GL_TRIANGLES, 0, circleResHor * circleResVert * 2 * 3, 0);
	vb_projectile.Off();

	for (std::pair<glm::vec3, float> &par : particles) {
		if (par.second > 0) {
			projectileParticle->Draw(camera, sh_projectile, par.first);
		}
	}
	sh_projectile.Off();
}

//Gömb parametrikus leírása: (u,v) -> (x,y,z)
glm::vec3 BShipProjectile::GetUV(float u, float v) {
	u *= 2 * glm::pi<float>();
	v *= glm::pi<float>();

	float cu = cosf(u);
	float su = sinf(u); 
	float cv = cosf(v);
	float sv = sinf(v);

	return glm::vec3(cu*sv, cv, su*sv);
}

ParticleGroup* BShipProjectile::getProjectileParticle() {
	return projectileParticle;
}