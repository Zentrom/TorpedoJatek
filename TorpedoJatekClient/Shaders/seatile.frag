#version 460

in vec3 vs_out_pos;
in vec4 vs_out_color;
in vec3 vs_out_normal;
in vec2 vs_out_tex;

out vec4 fs_out_col;

vec3 light_start = vec3(1, 1,-1); //F�ny fel� mutat� egys�gvektor
vec3 La = vec3(0.2f, 0.2f, 0.2f); //Ambiens f�nyer�
vec3 Ld = vec3(0.8f, 0.8f, 0.8f); //Diff�z f�nyer�
vec4 transparency = vec4(1, 1, 1, 0.7f); //4dik �rt�k az �tl�tsz�s�g

//vec3 Ls = vec3(1, 1, 0.4f); //Spekul�ris f�nyer�
//float specular_power = 128; //Spekul�ris sz�r�s
//uniform vec3 eye_pos = vec3(0, 20, 20); //Kamera pos

uniform bool hasTexture = false; //Text�ra van-e
uniform sampler2D texImage; //2Ds text�ra mintav�telez�

uniform bool tilestate_changed = false; //Alap�rt�kt�l elt�r�e a j�t�kmez� �llapota
uniform float seatileTexOffset = 0.0f; //Tengermez� text�ra offset
uniform vec3 tile_state; //J�t�kmez� �llapota

void main()
{
	vec3 normal = normalize(vs_out_normal);

	vec3 ambient = La;

	vec3 light_dir = normalize(light_start);
	float diff = max(dot( normal, light_dir), 0.0f);
	vec3 diffuse = Ld * diff;

	//vec3 viewDir = normalize( eye_pos - vs_out_pos );
	//vec3 reflection = reflect( -light_dir, normal );
	//float si = pow( max( dot(viewDir, reflection), 0.0f), specular_power );
	//vec3 specular = Ls * si;

	vec4 lightNoSpecular = vec4(ambient + diffuse, 1.0f);
	//vec4 light = vec4(lightNoSpecular.xyz + specular, 1.0f);

	vec2 newSeatexturePos = vec2(vs_out_tex.s + seatileTexOffset, vs_out_tex.t);
	vec4 texture_col = texture(texImage, newSeatexturePos);
	if(tilestate_changed){
		vec4 final_col = mix(texture_col, vec4(tile_state, 1), 0.5f);
		fs_out_col = transparency * lightNoSpecular * final_col;
	}else{
		fs_out_col = transparency * lightNoSpecular * texture_col;
	}

	//fs_out_col = light * ( hasTexture ? texture(texImage, vs_out_tex.st) : vs_out_color );

}