#version 460

in vec3 vs_out_pos;
in vec4 vs_out_color;

out vec4 fs_out_col;

uniform bool is_preprocess = true; //El�sz�m�t�si f�zis-e
uniform int tile_index = 1024; //Feldolgozott PlayTile idje

uniform int read_index = 1023; //Preprocessbe beolvasott sz�n
uniform vec3 tile_state; //PlayTile �llapot sz�ne

//PlayTile indexet �rja alphaba preprocessn�l
//Outlineokat kirajzolja m�s esetben
void main()
{
	if(is_preprocess){
		fs_out_col.a = tile_index;
	}else{
		if(tile_index == read_index){
			fs_out_col = vec4(0, 1, 1, 1); //Kijel�l�s sz�ne
		}else{
			fs_out_col = vec4(tile_state, 1);
		}
	}
}