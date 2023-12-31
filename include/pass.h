#ifndef PASS_H
#define PASS_H

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "vao.h"

// FBO 내부의 텍스처 id를 들고 있는 이유 -> 쉐이더 프로그램에서 써야하기 때문에.
// glGetUniformLocation <- 함수명이 길기도하고 느리다는 소문이 있어서 따로 location을 저장해두는 방안 고려할 것

/*
Geometry pass
- FBO: G-buffer
- uniform: MVP행렬
*/
typedef struct s_geometry_pass
{
	GLuint shader_program; // vertex shader + fragment shader
	
	// location in shader program
	GLuint loc_model;
	GLuint loc_view;
	GLuint log_proj;
	
	GLuint fbo; // G-Buffer (FBO)
	GLuint fbo_position; // 2D texture
	GLuint fbo_albedo; // 2D texture
	GLuint fbo_normal; // 2D texture
	
} t_geometry_pass;

/*
Lighting pass
- FBO: ?
- uniform: albedo, depth
*/
typedef struct s_lighting_pass
{
	t_geometry_pass *g_pass;
	GLuint shader_program; // only fragment shader

	// location in shader program
	GLuint loc_g_position;
	GLuint loc_g_normal;
	GLuint loc_g_albedo;
	GLuint loc_ambient;

	// GLuint fbo;
	// GLuint fbo_color; // 2D texture
} t_lighting_pass;

int lighting_pass_init(t_lighting_pass *l_pass);
int geometry_pass_init(t_geometry_pass *g_pass);
int geometry_pass_draw(const t_geometry_pass *g_pass, const t_vao *vao, bool is_default_fbo);
int lighting_pass_draw(const t_lighting_pass *l_pass, const t_vao *vao);

#endif
