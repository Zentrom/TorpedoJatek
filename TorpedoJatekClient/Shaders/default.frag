#version 460

in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D quadTexture;
								  
void main()
{
	fs_out_col = texture(quadTexture, vs_out_tex.st);
}