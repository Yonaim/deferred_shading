// 각 픽셀의 정보를 G-buffer로 출력 (G-buffer에 위치와 법선을 저장)

#version 330 core

in vec3 pos; // 프래그먼트 위치좌표 (World Space)
in vec3 normal; // 프래그먼트 법선벡터 (World Space)

out vec3 FragColor;
out vec3 g_position; // g-buffer에 저장하는 position
out vec3 g_normal;   // g-buffer에 저장하는 normal
out vec3 g_albedo;   // g-buffer에 저장하는 albedo

void main()
{
    g_position = pos;
    g_normal = normal;
    g_albedo = vec3(0, 0, 1);

    FragColor = vec3(0, 1, 0);
}
