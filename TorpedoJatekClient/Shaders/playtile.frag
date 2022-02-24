#version 460

in vec3 vs_out_pos;
in vec4 vs_out_color;

out vec4 fs_out_col;

uniform bool is_preprocess = true;
uniform int tile_index = 1024;

uniform int read_index = 1023;
uniform vec3 tile_state;

//Ez most csak a PlayTile indexet írja alphaba,szín mind1
void main()
{
	if(is_preprocess){
		fs_out_col = vec4(1, 1, 1, tile_index);
	}else{
		if(tile_index == read_index){
			fs_out_col = vec4(0, 1, 1, 1);
		}else{
			fs_out_col = vec4(tile_state, 1);
		}
	}
}