#version 460

in vec3 vs_in_pos;
in vec3 vs_in_color;
in vec3 vs_in_normal;
//in vec2 vs_in_tex0;

out vec3 vs_out_pos;
out vec3 vs_out_color;
out vec3 vs_out_normal;
//out vec2 vs_out_tex0;

uniform bool is_playtile = false;
uniform vec3 tile_state;
uniform mat4 world;
uniform mat4 worldIT;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );

	vs_out_pos = (world * vec4( vs_in_pos, 1 )).xyz;
	vs_out_normal  = (worldIT * vec4(vs_in_normal, 0)).xyz;
	if(is_playtile){
		vs_out_color = tile_state;
	}else{
		vs_out_color = vs_in_color;
	}
}