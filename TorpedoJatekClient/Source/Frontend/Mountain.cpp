
#include "Mountain.h"

const float Mountain::heightY = 8.0f;
const float Mountain::widthX = 6.0f;

Mountain::Mountain(void)
{

}

Mountain::~Mountain(void)
{
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

	// az indexek meghatározása minden négyzethez
	for (int i = 0; i < mountainResolution; ++i) {
		for (int j = 0; j < mountainResolution; ++j) {
			// az egyik háromszög a négyzet egyik fele
			vb_mountain.AddIndex((i + 1) + (j) * (mountainResolution + 1),
				(i) + (j + 1) * (mountainResolution + 1),
				(i)+(j) * (mountainResolution + 1)
			);
			// a másik háromszög a négyzet másik fele
			vb_mountain.AddIndex((i + 1) + (j + 1) * (mountainResolution + 1),
				(i) + (j + 1) * (mountainResolution + 1),
				(i + 1) + (j) * (mountainResolution + 1)
			);
		}
	}

	vb_mountain.InitBuffers();

	mountainTextureID = GLUtils::TextureFromFile("Resources/Textures/mountainTexture.bmp");
}

//Kirajzolja a hegyet
void Mountain::Draw(gCamera &camera, gShaderProgram &sh_program)
{
	glm::mat4 matWorld = glm::translate(mountain_translate) * glm::rotate(mountain_rotate, mountain_rotate_angle) * glm::scale(mountain_scale);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = camera.GetViewProj() *matWorld;

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
float Mountain::getWidth()
{
	return widthX;
}

float Mountain::getHeight()
{
	return heightY;
}
