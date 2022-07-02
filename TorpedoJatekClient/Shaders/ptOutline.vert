#version 460

in vec3 vs_in_pos;
in vec4 vs_in_color;

out vec3 vs_out_pos;
out vec4 vs_out_color;

uniform mat4 world;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );

	vs_out_pos = (world * vec4( vs_in_pos, 1 )).xyz;
	vs_out_color = vs_in_color;
}