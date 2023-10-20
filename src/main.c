#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "linalg.h"
#include "pass.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

int shader_program(GLuint *program, const char *vs_path, const char *fs_path);

int start_opengl(GLFWwindow **window_ptr)
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  	*window_ptr = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Deferred shading", NULL, NULL);
	if (*window_ptr == NULL)
		return (-1);
	glfwMakeContextCurrent(*window_ptr);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return (-1);
	return (0);
}

void init_vertices_cube(float *vertices_ptr[])
{
	float vertices[] = {
	// 위치					// 법선벡터
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	*vertices_ptr = malloc(sizeof(vertices));
	memcpy(*vertices_ptr, vertices, sizeof(vertices));
}

/*
1. 쉐이더 프로그램 컴파일
2. G-Buffer 생성 (position, normal, albedo, depth)
*/
int init_geometry_pass(geometry_pass *g_pass)
{
    // 쉐이더 프로그램
    if (shader_program(g_pass->shader_program, \
                    "./shader/geometry.vs", \
                    "./shader/geometry.fs") == -1)
	{
		fprintf(stderr, "Something wrong during make shader program");
		return (-1);
	}

	// FBO (G-buffer) 생성 및 바인딩.
	// FBO에는 texture 혹은 renderBuffer 객체를 연결할 수 있음
    glGenFramebuffers(1, &g_pass->g_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, g_pass->g_buffer);

    // gPosition 텍스처 생성 및 첨부
    glGenTextures(1, &g_pass->g_buffer_position);
    glBindTexture(GL_TEXTURE_2D, g_pass->g_buffer_position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_pass->g_buffer_position, 0);

    // gNormal 텍스처 생성 및 첨부
    glGenTextures(1, &g_pass->g_buffer_normal);
    glBindTexture(GL_TEXTURE_2D, g_pass->g_buffer_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_pass->g_buffer_normal, 0);

    // gAlbedo 텍스처 생성 및 첨부
    glGenTextures(1, &g_pass->g_buffer_albedo);
    glBindTexture(GL_TEXTURE_2D, g_pass->g_buffer_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_pass->g_buffer_albedo, 0);

    // G-Buffer에 깊이 버퍼 첨부 (텍스처가 아닌 RenderBuffer이다.)
    glGenRenderbuffers(1, &g_pass->g_buffer_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, g_pass->g_buffer_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIN_WIDTH, WIN_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_pass->g_buffer_depth);

    // G-Buffer에 사용될 color attachment 목록 설정
    // RenderBuffer는 알릴 필요가 없다.
    GLuint attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    // 첨부 완료 후 FBO 상태 확인
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is not complete");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 기본 프레임 버퍼로 복원

	return (0);
}

/*
1차 패스(geometry pass)에서 G-buffer를 채우고
2차 패스(lighting pass)에서 G-buffer의 내용물을 이용해 lighting을 한다. (익숙한 퐁 라이팅을 사용)

렌더링 루프:
    1. geometry pass: render all geometric/color data to g-buffer 
    2. lighting pass: use g-buffer to calculate the scene's lighting
*/
int main()
{
	GLFWwindow* window;
	if (start_opengl(&window) == -1)
		return (-1);

	float *vertices;
	init_vertices_cube(&vertices);

    geometry_pass g_pass;
    init_geometry_pass(&g_pass);

	// lighting pass 초기화

    // 기본값 세팅
	
    // 렌더링 루프
    // ...
}