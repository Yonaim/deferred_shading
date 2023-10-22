#include <GLFW/glfw3.h>

int compile_shader(GLuint *shader, char *path, int type);
int link_shaders(GLuint *program, GLuint vs, GLuint fs);
