#version 460
in vec3 vs_in_pos;

out vec3 TexCoords;

uniform mat4 viewProj;

void main()
{
	TexCoords = vs_in_pos;
	vec4 position = viewProj * vec4(vs_in_pos, 1.0);
	gl_Position = position.xyww;
}
