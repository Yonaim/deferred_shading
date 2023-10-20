// 각 픽셀의 정보를 G-buffer로 출력 (G-buffer에 위치와 법선을 저장)

#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 gPosition; // g-buffer에 저장하는 position
out vec4 gNormal;   // g-buffer에 저장하는 normal

void main()
{
    gPosition = vec4(FragPos, 1.0);
    gNormal = vec4(normalize(Normal), 0.0);
}
