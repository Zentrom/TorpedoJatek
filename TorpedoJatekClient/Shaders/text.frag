#version 460

in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform vec3 bgColor;
uniform sampler2D textTexture;
						  
void main()
{
	fs_out_col = texture(textTexture, vs_out_tex.st);

	if(fs_out_col.rgb == bgColor) discard;
}