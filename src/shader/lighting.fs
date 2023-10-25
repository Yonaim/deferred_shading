// G-buffer에서 필요한 정보를 읽고, 조명 및 그림자 효과를 계산
#version 330 core

in vec2 texture_coords; // 텍스처 좌표 (0~1)

out vec3 final_color; // 최종출력 컬러

// g-buffer부터 읽어온 텍스처들
uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo;

// 그 외 라이팅에 필요한 정보들
uniform vec3 ambient;
uniform vec3 camera_pos;
uniform vec3 light_pos;
uniform vec3 light_color;

vec3 diffuse;
vec3 specular;

vec3 reflect_vec;

void main()
{
	// g-buffer의 텍스처로부터 적절한 값을 얻어온다
	vec3 position = texture(g_position, texture_coords).rgb;
	vec3 normal = texture(g_normal, texture_coords).rgb;
	vec3 albedo = texture(g_albedo, texture_coords).rgb;
	
	vec3 l_vec = normalize(light_pos - position);

	// vec3 reflect_vec = reflect(L, normal);
	diffuse = max(dot(l_vec, -normal), 0) * light_color;

	// dot(R, N)
	// dot(Lz, N)

	specular = vec3(0, 0, 0);

	// final_color = diffuse + specular + ambient;
	// final_color = normal;
	final_color = l_vec;

}