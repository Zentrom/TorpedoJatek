#version 460

in vec3 vs_out_pos;
in vec4 vs_out_color;
in vec3 vs_out_normal;
in vec2 vs_out_tex;

out vec4 fs_out_col;

vec3 light_start = vec3(1, 1,-1);
vec3 La = vec3(0.2f, 0.2f, 0.2f);
vec3 Ld = vec3(0.8f, 0.8f, 0.8f);
vec3 Ls = vec3(1, 1, 0.4f);
float specular_power = 128;
vec4 transparency = vec4(1, 1, 1, 0.7f);

uniform vec3 eye_pos = vec3(0, 20, 20);
uniform bool hasTexture = false;
uniform sampler2D texImage;

uniform bool is_seatile = false;
uniform bool tilestate_changed = false;
uniform float seatileOffset = 0.0f;
uniform vec3 tile_state;

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

	vec4 lightNoSpecular = vec4(ambient + diffuse, 1.0f);
	vec4 light = vec4(ambient + diffuse + specular, 1.0f);

	if(is_seatile){
		vec2 newSeatexturePos = vec2(vs_out_tex.s + seatileOffset, vs_out_tex.t);
		vec4 texture_col = texture(texImage, newSeatexturePos);
		if(tilestate_changed){
			vec4 final_col = mix(texture_col, vec4(tile_state, 1), 0.5f);
			fs_out_col = transparency * lightNoSpecular * final_col;
		}else{
			fs_out_col = transparency * lightNoSpecular * texture_col;
		}
	}else{
		fs_out_col = light * ( hasTexture ? texture(texImage, vs_out_tex.st) : vs_out_color );
	}

}