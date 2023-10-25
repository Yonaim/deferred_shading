// 각 픽셀의 정보를 G-buffer로 출력 (G-buffer에 위치와 법선을 저장)

#version 330 core

in vec3 pos; // 프래그먼트 위치좌표 (World Space)
in vec3 normal; // 프래그먼트 법선벡터 (World Space)

out vec3 g_position; // g-buffer에 저장하는 position
// 디폴트 FBO일시 이게 최종 출력 색상임
out vec3 g_normal;   // g-buffer에 저장하는 normal
out vec3 g_albedo;   // g-buffer에 저장하는 albedo

void main()
{
    // g_position = vec3(-gl_FragCoord.x / 800, -gl_FragCoord.y / 600, 0);
    // g_position = vec3(0, 0, gl_FragCoord.z);

    g_position = pos;
    // g_position = normal;

    // g_position = pos;
    // g_normal = normal;
    // g_albedo = vec3(0.9373, 0.2667, 1.0);
}
