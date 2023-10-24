#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "vao.h"

static void init_vbo_cube(GLuint *vbo_pos, GLuint *vbo_normal)
{
	// TODO: EBO 적용하여 버텍스 수 24개로 줄이기
	float positions[] = {
    // 옆면1 (정면)
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    
    // 옆면2 (우측)
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,

    // 옆면3 (뒷면)
    0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    // 옆면4 (좌측)
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,

    // 윗면
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,

    // 아랫면
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,

	};

	float normals[] = {


    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f, 
    0.0f,  0.0f, -1.0f, 
    0.0f,  0.0f, -1.0f, 
    0.0f,  0.0f, -1.0f, 
    0.0f,  0.0f, -1.0f, 
    
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,

    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,

    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,

    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,

    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,

	};

	glGenBuffers(1, vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
}

void vao_init_cube(t_vao *vao_ptr)
{
	GLuint vbo[2];
	init_vbo_cube(&vbo[0], &vbo[1]);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 3, (void *)0);

	vao_ptr->id = vao;
	vao_ptr->vertices_cnt = 36;
}

void vao_init_quad(t_vao *vao_ptr)
{
	GLuint vbo;
    float positions[] = {
        1.0f, 1.0f, 
        -1.0f, 1.0f,  
        -1.0f, -1.0f,  

        1.0f, 1.0f,  
        -1.0f, -1.0f,  
        1.0f, -1.0f
    };
    glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)0);

	vao_ptr->id = vao;
	vao_ptr->vertices_cnt = 12;
}

void vao_init_triangle(t_vao *vao_ptr)
{
	float positions[] = {
        0.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f
	};
	float normals[] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
	};

    GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
    GLuint vbo_pos, vbo_normal;
	glGenBuffers(1, &vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 3, (void *)0);

	vao_ptr->id = vao;
	vao_ptr->vertices_cnt = 3;
}