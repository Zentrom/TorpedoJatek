#version 130

in vec3 vs_in_pos;
in vec3 vs_in_colorR;
in vec3 vs_in_colorG;
in vec3 vs_in_colorB;


out vec3 vs_out_pos;
out vec3 vs_out_color;

uniform vec3 tile_state;
uniform mat4 world;
uniform mat4 worldIT;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );

	vs_out_pos = (world * vec4( vs_in_pos, 1 )).xyz;
	vs_out_color = (vs_in_colorR * tile_state) + (vs_in_colorG * tile_state) + (vs_in_colorB * tile_state);
}