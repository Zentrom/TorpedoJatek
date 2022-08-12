#version 460

in vec4 vs_out_color;
in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform bool has_Texture = false;
uniform int clickableIndex = 512;
uniform int read_index = 511;
uniform sampler2D optionTexture;
				  
void main()
{
	if(has_Texture){
		fs_out_col = texture(optionTexture, vs_out_tex.st);
	}else{
		fs_out_col = vs_out_color;
	}
	if(clickableIndex == read_index){
		fs_out_col = mix(fs_out_col, vec4(0, 1.0f, 0, 1.0f), 0.5f);
	}
}