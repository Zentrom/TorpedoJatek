#version 460

in vec3 vs_out_pos;
in vec4 vs_out_color;
in vec3 vs_out_normal;
in vec2 vs_out_tex;

out vec4 fs_out_col;

vec3 light_start = vec3(1, 1,-1); //Fény felé mutató egységvektor
vec3 La = vec3(0.2f, 0.2f, 0.2f); //Ambiens fényerõ
vec3 Ld = vec3(0.8f, 0.8f, 0.8f); //Diffúz fényerõ
float transparencyMax = 0.7f; //Átlátszóság maximum

uniform float near = 0.001f; //Kamera közeli vágósík
uniform float far = 200.0f; //Kamera távoli vágósík

uniform bool hasTexture = false; //Textúra van-e
uniform sampler2D texImage; //2Ds textúra mintavételezõ

uniform bool tilestate_changed = false; //Alapértéktõl eltérõe a játékmezõ állapota
uniform float seatileTexOffset = 0.0f; //Tengermezõ textúra offset
uniform vec3 tile_state; //Játékmezõ állapota

void main()
{
	vec3 normal = normalize(vs_out_normal);

	vec3 ambient = La;

	vec3 light_dir = normalize(light_start);
	float diff = max(dot( normal, light_dir), 0.0f);
	vec3 diffuse = Ld * diff;

	vec4 lightNoSpecular = vec4(ambient + diffuse, 1.0f);

	vec2 newSeatexturePos = vec2(vs_out_tex.s + seatileTexOffset, vs_out_tex.t);
	vec4 texture_col = texture(texImage, newSeatexturePos);
	if(tilestate_changed){
		fs_out_col = lightNoSpecular * mix(texture_col, vec4(tile_state, 1), 0.5f);
	}else{
		fs_out_col = lightNoSpecular * texture_col;
	}

	float ndc = gl_FragCoord.z * 2.0f - 1.0f;
	float linearDepth = (2.0f * near * far) / (far + near - ndc * (far - near));
	float depth = linearDepth / far;

	//fs_out_col = vec4(vec3(depth), 1.0f);

	if(depth < 0.5f){
		fs_out_col.a = transparencyMax;
	}else{
		fs_out_col.a = mix(transparencyMax, 0.1f, depth * 2 - 1);
	}
	//fs_out_col.a = mix(transparencyMax, 0.1f, depth);
}