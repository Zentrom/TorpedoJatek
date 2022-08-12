#version 460

in vec4 vs_out_color;
in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform bool is_Decorator = false;
uniform bool is_PreProcess = false;
uniform int clickableIndex = 512;
uniform int read_index = 511;
uniform sampler2D textTexture;
				  
void main()
{
	if(is_PreProcess){
		fs_out_col.a = clickableIndex;
	}else{
		fs_out_col = texture(textTexture, vs_out_tex.st);

		if(fs_out_col.rgb == vec3(0, 0, 0)){
			if(is_Decorator) discard;
			else
			{
				fs_out_col = vs_out_color;
				if(clickableIndex == read_index){
					fs_out_col = mix(fs_out_col, vec4(0, 1.0f, 1.0f, 1.0f), 0.5f);
				}
			}
		}
	}
}