#version 330 core

in vec3 in_position;
in vec3 in_normal;

out vec3 pos; // 버텍스 위치좌표 (World Space)
out vec3 normal;  // 버텍스 법선벡터 (World Space)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection; // -w ~ w 사이값으로 맞춰줌, 그 밖 범위는 클리핑

void main()
{
    vec4 pos_v4 = model * vec4(in_position, 1.0);

    pos = pos_v4.xyz;
    normal = normalize((model * vec4(in_normal, 0.0)).xyz);

    gl_Position = projection * view * pos_v4;
}
