#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "pass.h"
#include "shader.h"
#include "settings.h"

int init_lighting_pass(lighting_pass *l_pass)
{
    // 쉐이더 프로그램
    GLuint vs, fs;
    compile_shader(&vs, "./src/shader/lighting.vs", GL_VERTEX_SHADER);
    compile_shader(&fs, "./src/shader/lighting.fs", GL_FRAGMENT_SHADER);
    if (link_shaders(&l_pass->shader_program, vs, fs) == -1)
	{
		fprintf(stderr, "Something wrong during make shader program\n");
		return (-1);
	}
	return (0);
}
