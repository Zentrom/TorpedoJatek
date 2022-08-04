#version 460

in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform bool is_PreProcess = false;
uniform int clickableIndex = 512;
uniform vec3 bgColor;
uniform sampler2D textTexture;
				  
void main()
{
	if(is_PreProcess){
		fs_out_col.a = clickableIndex;
	}else{
		fs_out_col = texture(textTexture, vs_out_tex.st);
	}

	//if(fs_out_col.rgb == bgColor) discard;
}