#version 460

in vec2 vs_in_pos;
in vec4 vs_in_color;
in vec2 vs_in_tex;

out vec4 vs_out_color;
out vec2 vs_out_tex;

uniform vec2 translation;

void main()
{
	gl_Position = vec4(vs_in_pos + translation, 0, 1);
	vs_out_color = vs_in_color;
	vs_out_tex = vs_in_tex;
}