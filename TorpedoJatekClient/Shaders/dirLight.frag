#version 460

in vec3 vs_out_pos;
in vec4 vs_out_color;
in vec3 vs_out_normal;
//in vec2 vs_out_tex0;
in vec2 vs_out_tex;

out vec4 fs_out_col;

vec3 light_start = vec3(1, 1,-1);
vec3 La = vec3(0.2f, 0.2f, 0.2f);
vec3 Ld = vec3(0.8f, 0.8f, 0.8f);
vec3 Ls = vec3(1, 1, 0.4f);
float specular_power = 64;

uniform vec3 eye_pos = vec3(0, 20, 20);
uniform bool hasTexture = false;
uniform sampler2D texImage;

void main()
{
	vec3 normal = normalize(vs_out_normal);

	vec3 ambient = La;

	vec3 light_dir = normalize(light_start);
	float diff = max(dot( normal, light_dir), 0.0f);
	vec3 diffuse = Ld * diff;

	vec3 viewDir = normalize( eye_pos - vs_out_pos );
	vec3 reflection = reflect( -light_dir, normal );
	float si = pow( max( dot(viewDir, reflection), 0.0f), specular_power );
	vec3 specular = Ls * si;

	vec4 light = vec4(ambient+diffuse+specular, 1.0f);

	fs_out_col = light * ( hasTexture ? texture(texImage, vs_out_tex.st) : vs_out_color );

}