#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "pass.h"
#include "shader.h"
#include "settings.h"

/*
1. 쉐이더 프로그램 컴파일
2. FBO(G-Buffer) 생성 (position, normal, albedo, depth)
*/
int init_geometry_pass(geometry_pass *g_pass)
{
    // 쉐이더 프로그램
    GLuint vs, fs;
    compile_shader(&vs, "./src/shader/geometry.vs", GL_VERTEX_SHADER);
    compile_shader(&fs, "./src/shader/geometry.fs", GL_FRAGMENT_SHADER);
    if (link_shaders(&g_pass->shader_program, vs, fs) == -1)
	{
		fprintf(stderr, "Something wrong during make shader program\n");
		return (-1);
	}
	// 프로그램의 변수에 FBO의 첨부물을 연결한다
	// 현재 바인딩되어있는 FBO의 첨부물이 순서대로 위치, 노말, 알베도이므로 0, 1, 2번째에 그대로 
	glBindFragDataLocation(g_pass->shader_program, 0, "g_position");
	glBindFragDataLocation(g_pass->shader_program, 1, "g_normal");
	glBindFragDataLocation(g_pass->shader_program, 2, "g_albedo");


	// FBO (G-buffer) 생성 및 바인딩.
	// FBO에는 texture 혹은 renderBuffer 객체를 연결할 수 있음
    glGenFramebuffers(1, &g_pass->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, g_pass->fbo);

    // g_position 텍스처 생성 및 첨부
    glGenTextures(1, &g_pass->fbo_position);
    glBindTexture(GL_TEXTURE_2D, g_pass->fbo_position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_pass->fbo_position, 0);

    // g_normal 텍스처 생성 및 첨부
    glGenTextures(1, &g_pass->fbo_normal);
    glBindTexture(GL_TEXTURE_2D, g_pass->fbo_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_pass->fbo_normal, 0);

    // g_albedo 텍스처 생성 및 첨부
    glGenTextures(1, &g_pass->fbo_albedo);
    glBindTexture(GL_TEXTURE_2D, g_pass->fbo_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_pass->fbo_albedo, 0);

    // G-Buffer에 깊이 버퍼 첨부 (텍스처가 아닌 RenderBuffer이다.)
    GLuint fbo_depth;
    glGenRenderbuffers(1, &fbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIN_WIDTH, WIN_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);

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
