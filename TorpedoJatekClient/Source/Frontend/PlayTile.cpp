
#include "PlayTile.h"

#include "../../Utils/gCamera.h"
#include "../../Utils/gShaderProgram.h"


PlayTile::PlayTile(void)
{
}

PlayTile::PlayTile(glm::vec3 translate)
{
	this->playtile_translate = translate;
}

PlayTile::~PlayTile(void)
{
}

void PlayTile::Init()
{
	vb_playtile.AddAttribute(0, 3);
	vb_playtile.AddAttribute(1, 3);

	vb_playtile.AddData(0, -0.5f, 0, -0.5f);
	vb_playtile.AddData(0, 0.5f, 0, -0.5f);
	vb_playtile.AddData(0, -0.5f, 0, 0.5f);
	vb_playtile.AddData(0, 0.5f, 0, 0.5f);
	
	vb_playtile.AddData(1, 0, 0, 1.0f);
	vb_playtile.AddData(1, 0, 0, 1.0f);
	vb_playtile.AddData(1, 0, 0, 1.0f);
	vb_playtile.AddData(1, 0, 0, 1.0f);
	
	vb_playtile.AddIndex(1, 0, 2);
	vb_playtile.AddIndex(1, 2, 3);
	
	vb_playtile.InitBuffers();
}

void PlayTile::Draw(gCamera &m_camera, gShaderProgram &m_program)
{
	glm::mat4 matWorld = glm::translate(playtile_translate) * glm::rotate(playtile_rotate, playtile_rotate_angle) * glm::scale(playtile_scale);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = m_camera.GetViewProj() *matWorld;

	m_program.SetUniform("world", matWorld);
	m_program.SetUniform("worldIT", matWorldIT);
	m_program.SetUniform("MVP", mvp);

	//m_program.SetTexture("texImage", 0, m_groundTextureID);
	//m_program.SetTexture("texNormal", 1, m_groundNormalMapID);

	vb_playtile.On();
	vb_playtile.DrawIndexed(GL_TRIANGLES, 0, 6, 0);
	vb_playtile.Off();
}