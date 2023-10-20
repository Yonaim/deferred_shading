#include <GLFW/glfw3.h>

typedef struct geometry_pass_
{
	GLuint shader_program; // vertex shader + fragment shader
	GLuint g_buffer; // G-Buffer (FBO)
	GLuint g_buffer_position; // 2D texture
		// 어차피 lighting pass에서는 사용 안 할텐데...저장해야하나?
	GLuint g_buffer_albedo; // 2D texture
	GLuint g_buffer_normal; // 2D texture
	GLuint g_buffer_depth; // render buffer
		// render buffer의 id까지 저장할 필요가 있을까?
} geometry_pass;

typedef struct lighting_pass_
{
	GLuint shader_program; // only fragment shader
	// ...
} lighting_pass;
