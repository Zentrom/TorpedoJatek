#version 460

in vec3 vs_in_pos;

//out vec3 vs_out_pos;
out vec3 vs_out_color;

uniform vec3 tile_state;
uniform mat4 world;
uniform mat4 worldIT;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );

	//vs_out_pos = (world * vec4( vs_in_pos, 1 )).xyz;
	vs_out_color = (vec3(1,0,0) * tile_state) + (vec3(0,1,0) * tile_state) + (vec3(0,0,1) * tile_state);
}