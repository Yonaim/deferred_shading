// gl_Position 값 세팅
// 텍스처 좌표를 gl_Position 값을 기반으로 적절히 세팅
// texture에 그려진 것을 기반으로 그리기를 수행하므로 라이팅패스에서는 삼각형이 아니라 사각형을 그린다고 할 수 있음.

#version 330 core

in vec2 position; // -1~1
out vec2 texture_coords; // 0~1

void main()
{
	gl_Position = vec4(position, 0, 1);
	texture_coords = position * 0.5 + 0.5;
}
