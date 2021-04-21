#version 460
in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube skyboxCube;

void main()
{
	FragColor = texture(skyboxCube, TexCoords);
}
