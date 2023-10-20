#include <GLFW/glfw3.h>

// FBO 내부의 텍스처 id를 들고 있는 이유 -> 쉐이더 프로그램에서 써야하기 때문에.

typedef struct geometry_pass_
{
	GLuint shader_program; // vertex shader + fragment shader
	GLuint g_buffer; // G-Buffer (FBO)
	GLuint g_buffer_position; // 2D texture
	GLuint g_buffer_albedo; // 2D texture
	GLuint g_buffer_normal; // 2D texture
} geometry_pass;

typedef struct lighting_pass_
{
	GLuint shader_program; // only fragment shader
	// ...
} lighting_pass;
