#version 460

in vec3 vs_out_pos;
in vec3 vs_out_color;
in vec3 vs_out_normal;
//in vec2 vs_out_tex0;

out vec4 fs_out_col;

vec3 light_start = vec3(1, 1,-1);
vec3 La = vec3(0.2f, 0.2f, 0.2f);
vec3 Ld = vec3(0.7f, 0.7f, 0.7f);
vec3 Ls = vec3(1, 1, 0.2f);
float specular_power = 64;

uniform vec3 eye_pos = vec3(0, 20, 20);
//uniform sampler2D texImage;

void main()
{
	vec3 normal = normalize(vs_out_normal);

	vec4 ambient = vec4(La, 1.0f);

	vec3 light_dir = normalize(light_start);
	float diff = max(dot( normal, light_dir), 0.0f);
	vec4 diffuse = vec4(Ld * diff,1.0f);

	vec3 viewDir = normalize( eye_pos - vs_out_pos );
	vec3 reflection = reflect( -light_dir, normal );
	float si = pow( max( dot(viewDir, reflection), 0.0f), specular_power );
	vec4 specular = vec4(Ls * si,1.0f);

	vec4 light = ambient + diffuse + specular;

	//fs_out_col = light * ( use_texture ? texture(texImage, vs_out_tex0.st) : vec4(1.0f) );
	fs_out_col = light * vec4(vs_out_color,1.0f);

}