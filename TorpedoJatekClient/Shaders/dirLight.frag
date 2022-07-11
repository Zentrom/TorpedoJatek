#version 460

in vec3 vs_out_pos;
in vec4 vs_out_color;
in vec3 vs_out_normal;
in vec2 vs_out_tex;

out vec4 fs_out_col;

vec3 light_start = vec3(1, 1,-1); //Fény felé mutató egységvektor
vec3 La = vec3(0.2f, 0.2f, 0.2f); //Ambiens fényerõ
vec3 Ld = vec3(0.8f, 0.8f, 0.8f); //Diffúz fényerõ
vec3 Ls = vec3(1, 1, 0.4f); //Spekuláris fényerõ
float specular_power = 128; //Spekuláris szórás

uniform bool is_ground = false; //Ground mezõ-e
uniform float near = 0.001f; //Kamera közeli vágósík
uniform float far = 200.0f; //Kamera távoli vágósík

uniform vec3 eye_pos = vec3(0, 20, 20); //Kamera pos
uniform bool hasTexture = true; //Textúra van-e
uniform sampler2D texImage; //2Ds textúra mintavételezõ

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

	vec4 light = vec4(ambient + diffuse + specular, 1.0f);

	fs_out_col = light * ( hasTexture ? texture(texImage, vs_out_tex.st) : vs_out_color );

	if(is_ground){
		float ndc = gl_FragCoord.z * 2.0f - 1.0f;
		float linearDepth = (2.0f * near * far) / (far + near - ndc * (far - near));
		float depth = linearDepth / far;
		//fs_out_col = vec4(vec3(depth), 1.0f);

		if(depth < 0.5f){
			fs_out_col.a = 1.0f;
		}else{
			fs_out_col.a = mix(1.0f, 0.0f, depth * 2 - 1);
		}
		//fs_out_col.a = mix(1.0f, 0.0f, depth);
	}
}