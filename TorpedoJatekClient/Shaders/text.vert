#version 460

in vec2 vs_in_pos;
in vec2 vs_in_tex;

out vec2 vs_out_tex;

uniform float lineWidth;
uniform float lineCount;

void main()
{
	float xPos = clamp(vs_in_pos.x * lineWidth, -1.0f, 1.0f);
	float yPos = clamp((vs_in_pos.y * lineCount) + 0.9f, -1.0f, 1.0f);

	gl_Position = vec4(xPos, yPos, 0, 1);
	vs_out_tex = vs_in_tex;
}