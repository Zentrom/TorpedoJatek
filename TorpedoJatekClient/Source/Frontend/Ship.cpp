
#include "Ship.h"

//BattleShip-nek fenntartva
Ship::Ship(bool ally) : isAlly(ally), shipFlag(new ShipFlag(ally))
{
}

Ship::Ship(const std::vector<PlayTile*> &tiles, bool ally) : playTiles(tiles), isAlly(ally),
	shipFlag(new ShipFlag(ally))
{
	if (tiles.at(0)) {
		glm::vec3 frontTranslation = tiles.at(0)->getTranslate();
		glm::vec3 backTranslation = tiles.at(tiles.size() - 1)->getTranslate();
		shipTranslate = (frontTranslation + backTranslation) / 2.0f;
		shipScale = glm::vec3(0.8f * tiles.size() * TorpedoGLOBAL::SeaTileScaleXZ,
			0.6f + 0.4f * tiles.size() * TorpedoGLOBAL::SeaTileScaleXZ,
			(0.3f + 0.12f*tiles.size()) * TorpedoGLOBAL::SeaTileScaleXZ )* TorpedoGLOBAL::Scale;
		if (tiles.size() > 1) {
			if (tiles.at(0)->getPos().first != tiles.at(1)->getPos().first) {
				shipRotate = glm::half_pi<float>();
			}
		}
	}
	
	Init();
}

Ship::~Ship()
{
	delete shipFlag;
	glDeleteTextures(1, &shipBottomTextureID);
	glDeleteTextures(1, &shipTopTextureID);
	vb_ship.Clean();
}

//Haj� modell adatainak inicializ�l�sa
void Ship::Init()
{
	matWorld = glm::translate(shipTranslate) * glm::rotate(shipRotate, shipRotateAngle) * glm::scale(shipScale)
		* glm::translate(shipAboveSeaTrans) * sinkTranslate * sinkRotate;
	matWorldIT = glm::transpose(glm::inverse(matWorld));

	vb_ship.AddAttribute(0, 3); //poz�ci�
	vb_ship.AddAttribute(1, 4); //sz�n
	vb_ship.AddAttribute(2, 3); //norm�lvektor
	vb_ship.AddAttribute(3, 2); //text�rakoord

	//Haj� als� pal�st
	vb_ship.AddData(0, -0.5f, 0, 0);
	vb_ship.AddData(0, -0.25f, -0.25f, 0);
	vb_ship.AddData(0, -0.25f, 0, 0.25f);
	vb_ship.AddData(0, -0.25f, 0, 0.25f);
	vb_ship.AddData(0, -0.25f, -0.25f, 0);
	vb_ship.AddData(0, 0.25f, 0, 0.25f);
	vb_ship.AddData(0, 0.25f, 0, 0.25f);
	vb_ship.AddData(0, -0.25f, -0.25f, 0);
	vb_ship.AddData(0, 0.25f, -0.25f, 0);
	vb_ship.AddData(0, 0.25f, 0, 0.25f);
	vb_ship.AddData(0, 0.25f, -0.25f, 0);
	vb_ship.AddData(0, 0.5f, 0, 0);
	//pal�st m�sik oldala
	vb_ship.AddData(0, 0.5f, 0, 0);
	vb_ship.AddData(0, 0.25f, -0.25f, 0);
	vb_ship.AddData(0, 0.25f, 0, -0.25f);
	vb_ship.AddData(0, 0.25f, 0, -0.25f);
	vb_ship.AddData(0, 0.25f, -0.25f, 0);
	vb_ship.AddData(0, -0.25f, 0, -0.25f);
	vb_ship.AddData(0, -0.25f, 0, -0.25f);
	vb_ship.AddData(0, 0.25f, -0.25f, 0);
	vb_ship.AddData(0, -0.25f, -0.25f, 0);
	vb_ship.AddData(0, -0.25f, 0, -0.25f);
	vb_ship.AddData(0, -0.25f, -0.25f, 0);
	vb_ship.AddData(0, -0.5f, 0, 0);
	//Fed�lap
	vb_ship.AddData(0, -0.25f, 0, -0.25f);
	vb_ship.AddData(0, -0.5f, 0, 0);
	vb_ship.AddData(0, -0.25f, 0, 0.25f);
	vb_ship.AddData(0, -0.25f, 0, -0.25f);
	vb_ship.AddData(0, -0.25f, 0, 0.25f);
	vb_ship.AddData(0, 0.25f, 0, 0.25f);
	vb_ship.AddData(0, 0.25f, 0, 0.25f);
	vb_ship.AddData(0, 0.25f, 0, -0.25f);
	vb_ship.AddData(0, -0.25f, 0, -0.25f);
	vb_ship.AddData(0, 0.25f, 0, -0.25f);
	vb_ship.AddData(0, 0.25f, 0, 0.25f);
	vb_ship.AddData(0, 0.5f, 0, 0);
	//G�las�tor
	vb_ship.AddData(0, -0.25f, 0, 0);
	vb_ship.AddData(0, 0, 0, 0.25f);
	vb_ship.AddData(0, 0, 0.25f, 0);
	vb_ship.AddData(0, 0, 0.25f, 0);
	vb_ship.AddData(0, 0, 0, 0.25f);
	vb_ship.AddData(0, 0.25f, 0, 0);
	vb_ship.AddData(0, 0.25f, 0, 0);
	vb_ship.AddData(0, 0, 0, -0.25f);
	vb_ship.AddData(0, 0, 0.25f, 0);
	vb_ship.AddData(0, 0, 0.25f, 0);
	vb_ship.AddData(0, 0, 0, -0.25f);
	vb_ship.AddData(0, -0.25f, 0, 0);

	if (!isAlly) {
		for (int i = 0; i < 48; ++i) {
			vb_ship.AddData(1, 1.0f, 0, 0, 1);
		}
	}
	else {
		for (int i = 0; i < 48; ++i) {
			vb_ship.AddData(1, 0, 1.0f, 0, 1);
		}
	}

	vb_ship.AddData(2, -0.5f, 0, 0);
	vb_ship.AddData(2, -0.25f, -0.25f, 0);
	vb_ship.AddData(2, -0.25f, 0, 0.25f);
	vb_ship.AddData(2, -0.25f, 0, 0.25f);
	vb_ship.AddData(2, -0.25f, -0.25f, 0);
	vb_ship.AddData(2, 0.25f, 0, 0.25f);
	vb_ship.AddData(2, 0.25f, 0, 0.25f);
	vb_ship.AddData(2, -0.25f, -0.25f, 0);
	vb_ship.AddData(2, 0.25f, -0.25f, 0);
	vb_ship.AddData(2, 0.25f, 0, 0.25f);
	vb_ship.AddData(2, 0.25f, -0.25f, 0);
	vb_ship.AddData(2, 0.5f, 0, 0);
	vb_ship.AddData(2, 0.5f, 0, 0);
	vb_ship.AddData(2, 0.25f, -0.25f, 0);
	vb_ship.AddData(2, 0.25f, 0, -0.25f);
	vb_ship.AddData(2, 0.25f, 0, -0.25f);
	vb_ship.AddData(2, 0.25f, -0.25f, 0);
	vb_ship.AddData(2, -0.25f, 0, -0.25f);
	vb_ship.AddData(2, -0.25f, 0, -0.25f);
	vb_ship.AddData(2, 0.25f, -0.25f, 0);
	vb_ship.AddData(2, -0.25f, -0.25f, 0);
	vb_ship.AddData(2, -0.25f, 0, -0.25f);
	vb_ship.AddData(2, -0.25f, -0.25f, 0);
	vb_ship.AddData(2, -0.5f, 0, 0);
	vb_ship.AddData(2, -0.25f, 0, -0.25f);
	vb_ship.AddData(2, -0.5f, 0, 0);
	vb_ship.AddData(2, -0.25f, 0, 0.25f);
	vb_ship.AddData(2, -0.25f, 0, -0.25f);
	vb_ship.AddData(2, -0.25f, 0, 0.25f);
	vb_ship.AddData(2, 0.25f, 0, 0.25f);
	vb_ship.AddData(2, 0.25f, 0, 0.25f);
	vb_ship.AddData(2, 0.25f, 0, -0.25f);
	vb_ship.AddData(2, -0.25f, 0, -0.25f);
	vb_ship.AddData(2, 0.25f, 0, -0.25f);
	vb_ship.AddData(2, 0.25f, 0, 0.25f);
	vb_ship.AddData(2, 0.5f, 0, 0);
	vb_ship.AddData(2, -0.25f, 0, 0);
	vb_ship.AddData(2, 0, 0, 0.25f);
	vb_ship.AddData(2, 0, 0.25f, 0);
	vb_ship.AddData(2, 0, 0.25f, 0);
	vb_ship.AddData(2, 0, 0, 0.25f);
	vb_ship.AddData(2, 0.25f, 0, 0);
	vb_ship.AddData(2, 0.25f, 0, 0);
	vb_ship.AddData(2, 0, 0, -0.25f);
	vb_ship.AddData(2, 0, 0.25f, 0);
	vb_ship.AddData(2, 0, 0.25f, 0);
	vb_ship.AddData(2, 0, 0, -0.25f);
	vb_ship.AddData(2, -0.25f, 0, 0);

	//Haj� als� pal�st
	vb_ship.AddData(3, 0, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);;
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);
	vb_ship.AddData(3, 0.75f, 1.0f);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.75f, 1.0f);
	vb_ship.AddData(3, 1.0f, 0);
	//pal�st m�sik oldala
	vb_ship.AddData(3, 0, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);;
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);
	vb_ship.AddData(3, 0.75f, 1.0f);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.75f, 1.0f);
	vb_ship.AddData(3, 1.0f, 0);
	//Fed�lap
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0, 0.5f);
	vb_ship.AddData(3, 0.25f, 1.0f);
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0.25f, 1.0f);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 0.75f, 1.0f);
	vb_ship.AddData(3, 0.25f, 0);
	vb_ship.AddData(3, 0.75f, 1.0f);
	vb_ship.AddData(3, 0.75f, 0);
	vb_ship.AddData(3, 1.0f, 0.5f);
	//G�las�tor
	vb_ship.AddData(3, 0, 1.0f);
	vb_ship.AddData(3, 0.5f, 1.0f);
	vb_ship.AddData(3, 0.5f, 0);
	vb_ship.AddData(3, 0.5f, 0);
	vb_ship.AddData(3, 0.5f, 1.0f);
	vb_ship.AddData(3, 1.0f, 1.0f);
	vb_ship.AddData(3, 1.0f, 1.0f);
	vb_ship.AddData(3, 0.5f, 1.0f);
	vb_ship.AddData(3, 0.5f, 0);
	vb_ship.AddData(3, 0.5f, 0);
	vb_ship.AddData(3, 0.5f, 1.0f);
	vb_ship.AddData(3, 0, 1.0f);

	vb_ship.InitBuffers();

	shipBottomTextureID = GLUtils::TextureFromFile("Resources/Textures/shipBottomTexture.bmp");
	shipTopTextureID = GLUtils::TextureFromFile("Resources/Textures/shipTopTexture.bmp");
}

//Haj� adatok friss�t�se real-time
void Ship::Update(float delta_time)
{
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

//Egy haj� kirajzol�sa
void Ship::Draw(const gCamera& camera, gShaderProgram& sh_program) const
{
	//glm::mat4 matWorld = glm::translate(shipTranslate) * glm::rotate(shipRotate, shipRotateAngle) * glm::scale(shipScale)
	//	* glm::translate(shipAboveSeaTrans) * sinkTranslate * sinkRotate;
	//glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
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
	shipFlag->Draw();
}

std::vector<PlayTile*>& Ship::getPlayTiles()
{
	return playTiles;
}

bool Ship::isDestroyed() const
{
	return destroyed;
}

void Ship::setDestroyed(bool dis)
{
	destroyed = dis;
}

bool Ship::isVisible() const
{
	return visible;
}

glm::vec3 Ship::getShipTranslate()
{
	return shipTranslate;
}
