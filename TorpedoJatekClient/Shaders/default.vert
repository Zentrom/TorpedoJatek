#version 460

in vec2 vs_in_pos;
in vec2 vs_in_tex;

out vec2 vs_out_tex;

uniform mat4 world;

void main()
{
	gl_Position = vec4(vs_in_pos, 0, 1);
	vs_out_tex = vs_in_tex;
}