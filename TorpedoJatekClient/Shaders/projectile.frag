#version 460

in vec3 vs_out_pos;
in vec3 vs_out_color;

out vec4 fs_out_col;

void main()
{
	fs_out_col = vec4(vs_out_color, 1.0f);
}