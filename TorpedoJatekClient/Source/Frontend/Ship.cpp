
#include "Ship.h"

Ship::Ship(void)
{
}

Ship::Ship(const std::vector<PlayTile*> &tiles, bool ally) : isAlly(ally), playTiles(tiles)
{
	Init();
	shipFlag = new ShipFlag(ally);

	if (tiles[0]) {
		glm::vec3 frontTranslation = tiles[0]->getTranslate();
		glm::vec3 backTranslation = tiles[tiles.size() - 1]->getTranslate();
		ship_translate = (frontTranslation + backTranslation) / 2.0f;
		ship_scale = glm::vec3(0.8f * tiles.size() * SeaTile::getScaleXZ(),
			0.6f + 0.4f*tiles.size() * SeaTile::getScaleXZ(), (0.3f + 0.12f*tiles.size()) * SeaTile::getScaleXZ() ) 
			* TorpedoGLOBAL::Scale;
		if (tiles.size() > 1) {
			if (tiles[0]->getPos().first != tiles[1]->getPos().first) {
				ship_rotate = glm::half_pi<float>();
			}
		}
	}
}

Ship::~Ship(void)
{
}

//Hajó modell adatainak inicializálása
void Ship::Init()
{
	vb_ship.AddAttribute(0, 3); //pozíció
	vb_ship.AddAttribute(1, 4); //szín
	vb_ship.AddAttribute(2, 3); //normálvektor
	vb_ship.AddAttribute(3, 2); //textúrakoord

	//Hajó alsó palást
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
	//palást másik oldala
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
	//Fedõlap
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
	//Gúlasátor
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
		for (int i = 0; i < 48; i++) {
			vb_ship.AddData(1, 1.0f, 0, 0, 1);
		}
	}
	else {
		for (int i = 0; i < 48; i++) {
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

	//Hajó alsó palást
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
	//palást másik oldala
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
	//Fedõlap
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
	//Gúlasátor
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

//Hajó adatok frissítése real-time
void Ship::Update(float deltatime)
{
	if (visible && destroyed) {
		sinkElapsed += deltatime;
		if (sinkElapsed > sinkTime) {
			visible = false;
		}
	}
}

//Egy hajó kirajzolása
void Ship::Draw(gCamera &camera, gShaderProgram &sh_program)
{
	sinkTranslate = glm::translate(glm::vec3(0, -(sinkElapsed / sinkTime) / 2.0f, 0));
	sinkRotate = glm::rotate((sinkElapsed / sinkTime) * glm::half_pi<float>() / 2.0f, glm::vec3(0, 0, 1.0f));

	glm::mat4 matWorld = glm::translate(ship_translate) * glm::rotate(ship_rotate, ship_rotate_angle) * glm::scale(ship_scale)
		* glm::translate(ship_abovesea_trans) * sinkTranslate * sinkRotate;
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	sh_program.SetUniform("eye_pos", camera.GetEye());

	sh_program.SetUniform("hasTexture", true);
	sh_program.SetTexture("texImage", 0, shipBottomTextureID);

	vb_ship.On();
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

bool Ship::isDestroyed()
{
	return destroyed;
}

void Ship::setDestroyed(bool dis)
{
	destroyed = dis;
}

bool Ship::isVisible()
{
	return visible;
}
