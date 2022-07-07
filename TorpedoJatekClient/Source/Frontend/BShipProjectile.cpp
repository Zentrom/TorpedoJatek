#include "BShipProjectile.h"

//cannon_shared_trans mátrixból az utsó oszlop elsõ 3 sora a mozgatás
BShipProjectile::BShipProjectile(const glm::mat4 &cannon_shared_trans, bool ally) : 
	startPos(glm::vec3(cannon_shared_trans[3][0], cannon_shared_trans[3][1], cannon_shared_trans[3][2])),
	isAlly(ally), projectileParticle(new ParticleGroup(projectileScale.x)), postAnimParticleTime(particleLife)
{
	currentPos = startPos;
	Init();
}

BShipProjectile::~BShipProjectile()
{
	vb_projectile.Clean();
	sh_projectile.Clean();
	delete projectileParticle;
}

//Lövedék modell adatainak inicializálása
void BShipProjectile::Init()
{
	vb_projectile.AddAttribute(0, 3); //pos
	vb_projectile.AddAttribute(1, 3); //color
	vb_projectile.AddAttribute(2, 3); //normal

	for (int i = 0; i <= circleResHor; ++i) {
		for (int j = 0; j <= circleResVert; ++j) {
			float u = i / static_cast<float>(circleResHor);
			float v = j / static_cast<float>(circleResVert);

			//Parametrikus koordinátákat konvertáljuk
			vb_projectile.AddData(0, GetUV(u, v));
			//szín
			vb_projectile.AddData(1, 1.0f, 0.5f, 0);
			//normálvektor
			vb_projectile.AddData(2, GetUV(u, v));
		}
	}
	//Hor*Vert db négyzet
	for (int i = 0; i < circleResHor; ++i) {
		for (int j = 0; j < circleResVert; ++j) {
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
	if (!sh_projectile.LinkProgram()) {
		std::cout << "[Shader_Link]Error during Shader compilation: sh_projectile" << std::endl;
	}
}

//Animációhoz szükséges elõkészítések
void BShipProjectile::Fire(const glm::vec3& shot_tile_pos)
{
	targetPos = shot_tile_pos;
	dist.x = glm::distance<float>(currentPos.x, targetPos.x);
	dist.y = dist.x / distHeightDivFactor;
	dist.z = targetPos.z - currentPos.z;

	dist.x *= (isAlly ? 1.0f : -1.0f);

}

//Animáció - igazat ad vissza ha még tart
bool BShipProjectile::Animate(float delta_time)
{
	elapsedTime += delta_time;
	if (elapsedTime < animationTime) {
		float angle = elapsedTime / animationTime *  glm::pi<float>();
		float fraction = elapsedTime / animationTime;
		currentPos = startPos + glm::vec3(fraction * dist.x, sinf(angle) * dist.y, fraction * dist.z);

		particleElapsed += delta_time;
		if (particleElapsed > particleEmitTime) {
			particleElapsed = 0.0f;
			particles.push_back(std::pair<const glm::vec3,float>(currentPos, particleLife));
		}
		for (std::pair<const glm::vec3, float>& par : particles) {
			if (par.second > 0) {
				par.second -= delta_time;
			}
		}
		return true;
	}
	else {
		elapsedTime = 0.0f;
		particleElapsed = 0.0f;
		postAnimParticleElapsed = 0.0f;
		currentPos = startPos;
	}
	return false;
}

//Lövés után élõ részecskék animációja
bool BShipProjectile::AnimPostParticles(float delta_time)
{
	postAnimParticleElapsed += delta_time;
	if (postAnimParticleElapsed < postAnimParticleTime) {
		for (std::pair<const glm::vec3, float>& par : particles) {
			if (par.second > 0) {
				par.second -= delta_time;
			}
		}
		return true;
	}
	return false;
}

//Részecske adatokat törli
void BShipProjectile::ClearParticles()
{
	particles.clear();
}

//Lövedék kirajzolása
void BShipProjectile::Draw(const gCamera& camera,const glm::mat4& shared_trans)
{
	glm::mat4 matWorld = glm::translate(currentPos) * shared_trans * glm::scale(projectileScale);
	glm::mat4 mvp = camera.GetViewProj() * matWorld;

	sh_projectile.On();

	sh_projectile.SetUniform("world", matWorld);
	sh_projectile.SetUniform("MVP", mvp);

	vb_projectile.On();
	vb_projectile.DrawIndexed(GL_TRIANGLES, 0, circleResHor * circleResVert * 2 * 3);
	vb_projectile.Off();

	for (std::pair<const glm::vec3, float> &par : particles) {
		if (par.second > 0) {
			projectileParticle->Draw(camera, sh_projectile, par.first);
		}
	}
	sh_projectile.Off();
}

float BShipProjectile::getAnimationTime() const
{
	return animationTime;
}

//Gömb parametrikus leírása: (u,v) -> (x,y,z)
glm::vec3 BShipProjectile::GetUV(float u, float v) {
	u *= 2 * glm::pi<float>();
	v *= glm::pi<float>();

	float cu = cosf(u);
	float su = sinf(u); 
	float cv = cosf(v);
	float sv = sinf(v);

	return glm::vec3(cu * sv, cv, su * sv);
}