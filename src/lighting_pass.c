#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "pass.h"
#include "settings.h"

int init_lighting_pass(lighting_pass *l_pass)
{
    // 쉐이더 프로그램
    if (shader_program(&l_pass->shader_program, \
                    "./src/shader/lighting.vs", \
                    "./src/shader/lighting.fs") == -1)
	{
		fprintf(stderr, "Something wrong during make shader program\n");
		return (-1);
	}
	return (0);
}
