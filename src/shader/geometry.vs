// 버텍스의 위치, 법선에 MVP 행렬 적용

#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

out vec3 FragPos; // 버텍스의 위치 (World Space)
out vec3 Normal;  // 버텍스의 법선 (World Space)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // 버텍스의 위치를 변환
    vec4 fragPos = model * vec4(inPosition, 1.0);
    gl_Position = projection * view * fragPos;

    // 버텍스의 법선을 변환
    Normal = mat3(transpose(inverse(model))) * inNormal;
    FragPos = vec3(fragPos);
}
