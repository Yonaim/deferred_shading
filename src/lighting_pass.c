#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "pass.h"
#include "shader.h"
#include "settings.h"
#include "vao.h"

int lighting_pass_init(t_lighting_pass *l_pass)
{
    // 쉐이더 프로그램
    GLuint vs, fs;
    compile_shader(&vs, "./src/shader/lighting.vs", GL_VERTEX_SHADER);
    compile_shader(&fs, "./src/shader/lighting.fs", GL_FRAGMENT_SHADER);
    l_pass->shader_program = glCreateProgram();
    // 프로그램 입력 변수에 속성 번호 지정
    glBindAttribLocation(l_pass->shader_program, 0, "position");
    if (link_shaders(l_pass->shader_program, vs, fs) == -1)
	{
		fprintf(stderr, "Something wrong during make shader program\n");
		return (-1);
	}
	return (0);
}

int lighting_pass_draw(const t_lighting_pass *l_pass, const t_vao *vao)
{
    // fbo 바인드 및 뷰포트 설정
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 일단 디폴트 프레임버퍼에 그림
    glViewport(0, 0, WIN_WIDTH * 2, WIN_HEIGHT * 2);
    glUseProgram(l_pass->shader_program);
    
    // 유니폼 값 세팅
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, l_pass->g_pass->fbo_position);
    glUniform1i(glGetUniformLocation(l_pass->shader_program, "g_position"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, l_pass->g_pass->fbo_normal);
    glUniform1i(glGetUniformLocation(l_pass->shader_program, "g_normal"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, l_pass->g_pass->fbo_albedo);
    glUniform1i(glGetUniformLocation(l_pass->shader_program, "g_albedo"), 2);

    glUniform3f(glGetUniformLocation(l_pass->shader_program, "ambient"), 0, 0, 0);
    glUniform3f(glGetUniformLocation(l_pass->shader_program, "camera_pos"), 0, 0, 0);
    glUniform3f(glGetUniformLocation(l_pass->shader_program, "light_pos"), 0, 0, 0);
    glUniform3f(glGetUniformLocation(l_pass->shader_program, "light_color"), 1, 1, 1);

    // 드로잉
    glBindVertexArray(vao->id);
    glDrawArrays(GL_TRIANGLES, 0, vao->vertices_cnt);
    return (0);
}
