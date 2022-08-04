#version 460

in vec3 vs_out_pos;
in vec4 vs_out_color;

out vec4 fs_out_col;

uniform bool is_preprocess = true; //Elõszámítási fázis-e
uniform int tile_index = 1024; //Feldolgozott PlayTile idje

uniform int read_index = 1023; //Preprocessbe beolvasott szín
uniform vec3 tile_state; //PlayTile állapot színe

//PlayTile indexet írja alphaba preprocessnél
//Outlineokat kirajzolja más esetben
void main()
{
	if(is_preprocess){
		fs_out_col.a = tile_index;
	}else{
		if(tile_index == read_index){
			fs_out_col = vec4(0, 1, 1, 1); //Kijelölés színe
		}else{
			fs_out_col = vec4(tile_state, 1);
		}
	}
}