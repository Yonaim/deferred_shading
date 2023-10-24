#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>
#include "pass.h"
#include "shader.h"
#include "settings.h"
#include "linalg.h"
#include "vao.h"

/*
1. 쉐이더 프로그램 컴파일
2. FBO(G-Buffer) 생성 (position, normal, albedo, depth)
*/
int geometry_pass_init(t_geometry_pass *g_pass)
{
    // 쉐이더 프로그램
    GLuint vs, fs;
    compile_shader(&vs, "./src/shader/geometry.vs", GL_VERTEX_SHADER);
    compile_shader(&fs, "./src/shader/geometry.fs", GL_FRAGMENT_SHADER);
    g_pass->shader_program = glCreateProgram();
    // 프로그램 입력 변수에 속성 번호 지정
    glBindAttribLocation(g_pass->shader_program, 0, "in_position");
    glBindAttribLocation(g_pass->shader_program, 1, "in_normal");
	// 프로그램 출력 변수에 컬러 버퍼 번호 지정
	// glBindFragDataLocation(g_pass->shader_program, 0, "FinalColor"); // 바인딩된 fbo의 0번째 첨부물을 g_position 변수에 바인딩
	glBindFragDataLocation(g_pass->shader_program, 0, "g_position"); // 바인딩된 fbo의 0번째 첨부물을 g_position 변수에 바인딩
	glBindFragDataLocation(g_pass->shader_program, 1, "g_normal"); // 바인딩된 fbo의 1번째 첨부물을 g_normal 변수에 바인딩
	glBindFragDataLocation(g_pass->shader_program, 2, "g_albedo"); // 바인딩된 fbo의 2번째 첨부물을 g_albedo 변수에 바인딩
    if (link_shaders(g_pass->shader_program, vs, fs) == -1)
	{
		fprintf(stderr, "Something wrong during make shader program\n");
		return (-1);
	}

	// FBO (G-buffer) 생성 및 바인딩.
	// FBO에는 texture 혹은 renderBuffer 객체를 연결할 수 있음
    glGenFramebuffers(1, &g_pass->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, g_pass->fbo);

    // g_position 텍스처 생성 및 첨부
    glGenTextures(1, &g_pass->fbo_position);
    glBindTexture(GL_TEXTURE_2D, g_pass->fbo_position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_pass->fbo_position, 0);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // g_normal 텍스처 생성 및 첨부
    glGenTextures(1, &g_pass->fbo_normal);
    glBindTexture(GL_TEXTURE_2D, g_pass->fbo_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_pass->fbo_normal, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // g_albedo 텍스처 생성 및 첨부
    glGenTextures(1, &g_pass->fbo_albedo);
    glBindTexture(GL_TEXTURE_2D, g_pass->fbo_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_pass->fbo_albedo, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

int geometry_pass_draw(const t_geometry_pass *g_pass, const t_vao *vao, bool is_default_fbo)
{
    // fbo 바인딩 및 뷰포트 설정
    if (is_default_fbo)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WIN_WIDTH * 2, WIN_HEIGHT * 2);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, g_pass->fbo);
        glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    }
    glUseProgram(g_pass->shader_program);

    // 유니폼 값 세팅
    float model_v[16], view_v[16], proj_v[16];
    model(model_v, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1)); 
    view(view_v, vec3(1, 1, -3), vec3(0, 0, 0)); 
    projection(proj_v, 1, 0.1, 100);
    glUniformMatrix4fv(glGetUniformLocation(g_pass->shader_program, "model"), 1, GL_FALSE, model_v);
    glUniformMatrix4fv(glGetUniformLocation(g_pass->shader_program, "view"), 1, GL_FALSE, view_v);
    glUniformMatrix4fv(glGetUniformLocation(g_pass->shader_program, "projection"), 1, GL_FALSE, proj_v);

    // 드로잉
    glBindVertexArray(vao->id);
    glDrawArrays(GL_TRIANGLES, 0, vao->vertices_cnt);
}
