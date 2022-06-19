
#include "Mountain.h"

const float Mountain::heightY = 8.0f;
const float Mountain::widthX = 6.0f;

Mountain::Mountain() : mountainScale(glm::vec3(widthX, heightY, widthZ) * TorpedoGLOBAL::Scale),
	matWorld(glm::scale(mountainScale)), matWorldIT(glm::transpose(glm::inverse(matWorld)))
{
}

Mountain::~Mountain()
{
	vb_mountain.Clean();
	glDeleteTextures(1, &mountainTextureID);
}

//Inicializálja a hegy grafikai modelljét
void Mountain::Init()
{
	vb_mountain.AddAttribute(0, 3); //vertexpos
	vb_mountain.AddAttribute(1, 4); //szín
	vb_mountain.AddAttribute(2, 3); //normálvektor
	vb_mountain.AddAttribute(3, 2); //textúracoord

	float vertexStep = 1.0f / static_cast<float>(mountainResolution);
	float xAxis;
	float zAxis;
	float yAxis;
	for (int x = 0; x <= mountainResolution; ++x) {
		for (int z = 0; z <= mountainResolution; ++z) {

			xAxis = vertexStep * x - 0.5f;
			zAxis = vertexStep * z - 0.5f;
			yAxis = glm::cos(glm::pi<float>() * xAxis) * glm::cos(glm::pi<float>() * zAxis) - 0.5f;

			vb_mountain.AddData(0, xAxis, yAxis, zAxis);
			vb_mountain.AddData(1, 0.5f, 0.5f, 0, 1);
			vb_mountain.AddData(2, xAxis, glm::abs(yAxis), zAxis);
			vb_mountain.AddData(3, vertexStep * z, vertexStep * x);
		}
	}

	//az indexek meghatározása minden négyzethez
	for (int i = 0; i < mountainResolution; ++i) {
		for (int j = 0; j < mountainResolution; ++j) {
			//négyzet egyik fele
			vb_mountain.AddIndex((i + 1) + j * (mountainResolution + 1),
				i + (j + 1) * (mountainResolution + 1),
				i + j * (mountainResolution + 1)
			);
			//négyzet másik fele
			vb_mountain.AddIndex((i + 1) + (j + 1) * (mountainResolution + 1),
				i + (j + 1) * (mountainResolution + 1),
				(i + 1) + j * (mountainResolution + 1)
			);
		}
	}

	vb_mountain.InitBuffers();

	mountainTextureID = GLUtils::TextureFromFile("Resources/Textures/mountainTexture.bmp");
}

//Kirajzolja a hegyet
void Mountain::Draw(const gCamera& camera, gShaderProgram& sh_program) const
{
	glm::mat4 mvp = camera.GetViewProj() * matWorld;

	sh_program.SetUniform("world", matWorld);
	sh_program.SetUniform("worldIT", matWorldIT);
	sh_program.SetUniform("MVP", mvp);
	sh_program.SetUniform("eye_pos", camera.GetEye());

	sh_program.SetUniform("hasTexture", true);
	sh_program.SetTexture("texImage", 0, mountainTextureID);

	vb_mountain.On();
	vb_mountain.DrawIndexed(GL_TRIANGLES, 0, 2 * 3 * mountainResolution * mountainResolution, 0);
	vb_mountain.Off();

	sh_program.SetUniform("hasTexture", false);
}

//Visszaadja hegy szélességét
float Mountain::getWidthX()
{
	return widthX;
}

float Mountain::getHeight()
{
	return heightY;
}

const float Mountain::getWidthZ() const
{
	return widthZ;
}
