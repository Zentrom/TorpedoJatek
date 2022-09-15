
#include "Ship.h"

//BattleShip-nek fenntartva
Ship::Ship(bool ally) : isAlly(ally)
{
}

Ship::Ship(const std::vector<PlayTile*> &tiles, bool ally) : pPlayTiles(tiles), isAlly(ally)
{
	if (tiles.at(0)) {
		glm::vec3 frontTranslation = tiles.at(0)->getTranslate();
		glm::vec3 backTranslation = tiles.at(tiles.size() - 1)->getTranslate();
		shipTranslate = (frontTranslation + backTranslation) / 2.0f;
		shipScale = glm::vec3(0.8f * tiles.size() * TorpedoGLOBAL::SeaTileScaleXZ,
			0.6f + 0.4f * tiles.size() * TorpedoGLOBAL::SeaTileScaleXZ,
			(0.3f + 0.12f*tiles.size()) * TorpedoGLOBAL::SeaTileScaleXZ )* TorpedoGLOBAL::Scale;
		if (tiles.size() > 1) {
			if (tiles.at(0)->getPos().first > tiles.at(1)->getPos().first) {
				shipRotate = -1*(glm::half_pi<float>());
			}
			else if(tiles.at(0)->getPos().first < tiles.at(1)->getPos().first){
				shipRotate = glm::half_pi<float>();
			}
		}
	}
	
	Init();
}

Ship::~Ship()
{
	glDeleteTextures(1, &shipBottomTextureID);
	glDeleteTextures(1, &shipTopTextureID);
}

//Hajó inicializálása
void Ship::Init()
{
	matWorld = glm::translate(shipTranslate) * glm::rotate(shipRotate, shipRotateAngle) * glm::scale(shipScale)
		* glm::translate(shipAboveSeaTrans) * sinkTranslate * sinkRotate;
	matWorldIT = glm::transpose(glm::inverse(matWorld));

	shipBottomTextureID = GLUtils::TextureFromFile("Resources/Textures/shipBottomTexture.bmp");
	shipTopTextureID = GLUtils::TextureFromFile("Resources/Textures/shipTopTexture.bmp");
}

//Hajó adatok frissítése real-time
void Ship::Update(float delta_time)
{
	if (preSinkTime) {
		preSinkElapsed += delta_time;
		if (preSinkElapsed > preSinkTime) {
			preSinkTime = 0.0f;
			destroyed = true;
		}
	}

	if (visible && destroyed) {
		sinkElapsed += delta_time;
		if (sinkElapsed > sinkTime) {
			visible = false;
		}
		sinkTranslate = glm::translate(glm::vec3(0, -(sinkElapsed / sinkTime) / 2.0f, 0));
		sinkRotate = glm::rotate((isAlly ? 1 : -1) * (sinkElapsed / sinkTime) * glm::half_pi<float>() / 2.0f, 
			glm::vec3(0, 0, 1.0f));

		matWorld = glm::translate(shipTranslate) * glm::rotate(shipRotate, shipRotateAngle) * glm::scale(shipScale)
			* glm::translate(shipAboveSeaTrans) * sinkTranslate * sinkRotate;
		matWorldIT = glm::transpose(glm::inverse(matWorld));
	}
}

//Egy hajó kirajzolása
void Ship::Draw(const gCamera& camera, gShaderProgram& sh_program, const gVertexBuffer& vb_ship, const gVertexBuffer& vb_flag) const
{
	glm::mat4 mvp = camera.GetViewProj() * matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	sh_program.SetUniform("eye_pos", camera.GetEye());

	sh_program.SetUniform("hasTexture", true);

	vb_ship.On();
	sh_program.SetTexture("texImage", 0, shipBottomTextureID);
	vb_ship.Draw(GL_TRIANGLES, 0, 36);
	sh_program.SetTexture("texImage", 0, shipTopTextureID);
	vb_ship.Draw(GL_TRIANGLES, 36, 12);
	vb_ship.Off();
	sh_program.SetUniform("hasTexture", false);
	
	vb_flag.On();
	glDisable(GL_CULL_FACE);
	vb_flag.DrawIndexed(GL_TRIANGLES, 0, 30);
	glEnable(GL_CULL_FACE);
	vb_flag.Off();
}

std::vector<PlayTile*>& Ship::getPlayTiles()
{
	return pPlayTiles;
}

bool Ship::isDestroyed() const
{
	return destroyed;
}

void Ship::setDestroyed(bool dis)
{
	destroyed = dis;
}

void Ship::setShotAt(float animation_time)
{
	preSinkTime = animation_time;
}

bool Ship::isVisible() const
{
	return visible;
}

glm::vec3 Ship::getShipTranslate()
{
	return shipTranslate;
}
