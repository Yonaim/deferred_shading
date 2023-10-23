#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "linalg.h"
#include "pass.h"
#include "vao.h"
#include "settings.h"

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

/* 
1차 패스(geometry pass)에서 G-buffer를 채우고
2차 패스(lighting pass)에서 G-buffer의 내용물을 이용해 lighting을 한다. (익숙한 퐁 라이팅을 사용)

렌더링 루프에 진입하기 전 초기화 작업으로서 각 pass에 필요한 FBO와
해당 FBO에 대한 첨부 대상물(attachments)를 구성하고, 필요한 모든 텍스처를 연결해둔다.
*/
int main()
{
	GLFWwindow* window;
	if (start_opengl(&window) == -1)
		return (-1);

    geometry_pass g_pass;
    init_geometry_pass(&g_pass);

    lighting_pass l_pass;
    init_lighting_pass(&l_pass);

	GLuint cube_vao;
	init_vao_cube(&cube_vao);

	GLuint quad_vao;
    init_vao_quad(&quad_vao);

	float model_v[16];
	float view_v[16];
	float proj_v[16];

// 렌더링 루프
	// 각 VAO들이 기하 패스를 모두 거치게끔 하여 g-buffer에 데이터를 쌓은 후 라이팅 패스를 거치도록한다
	// geometry pass는 각 오브젝트 별로 한번씩 수행, 최상단 픽셀에 대한 정보가 모아졌으니 lighting pass는 한번만 수행
    while (!glfwWindowShouldClose(window))
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0, 0.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 지오메트리 패스
        // glBindFramebuffer(GL_FRAMEBUFFER, g_pass.fbo);
        glUseProgram(g_pass.shader_program);
        // uniform 설정
		model(model_v, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1)); 
		view(view_v, vec3(0, 1, -3), vec3(0, 0, -1)); 
		projection(proj_v, 1, 0.1, 100); 
		glUniformMatrix4fv(glGetUniformLocation(g_pass.shader_program, "model"), 1, GL_FALSE, model_v);
		glUniformMatrix4fv(glGetUniformLocation(g_pass.shader_program, "view"), 1, GL_FALSE, view_v);
		glUniformMatrix4fv(glGetUniformLocation(g_pass.shader_program, "projection"), 1, GL_FALSE, proj_v);
        // vao 바인드
		glBindVertexArray(cube_vao);	
		// fbo에 draw
		glDrawArrays(GL_TRIANGLES, 0, 36);

// 라이팅 패스
        // glBindFramebuffer(GL_FRAMEBUFFER, 0); // 일단 디폴트 프레임 버퍼에 그림
        // glUseProgram(l_pass.shader_program);
        // // uniform 설정
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, g_pass.fbo_position);
        // glUniform1i(glGetUniformLocation(l_pass.shader_program, "g_position"), 0);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, g_pass.fbo_normal);
        // glUniform1i(glGetUniformLocation(l_pass.shader_program, "g_normal"), 1);
        // glActiveTexture(GL_TEXTURE2);
        // glBindTexture(GL_TEXTURE_2D, g_pass.fbo_albedo);
        // glUniform1i(glGetUniformLocation(l_pass.shader_program, "g_albedo"), 2);
        // glUniform3f(glGetUniformLocation(l_pass.shader_program, "ambient"), 0, 0, 0);
		// glUniform3f(glGetUniformLocation(l_pass.shader_program, "camera_pos"), 0, 0, 0);
		// glUniform3f(glGetUniformLocation(l_pass.shader_program, "light_pos"), 0, 0, 0);
		// glUniform3f(glGetUniformLocation(l_pass.shader_program, "light_color"), 1, 1, 1);
        // // vao 바인드
        // glBindVertexArray(quad_vao);
		// // fbo에 draw
		// // 디폴트 프레임버퍼 그 자체가 창이므로 draw하기만 하면 끝 (별도의 함수호출필요x)
		// glDrawArrays(GL_TRIANGLES, 0, 12);

        // 프레임 스와핑 및 이벤트 처리
		glfwSwapBuffers(window);
		glfwPollEvents();
    }
	glfwTerminate();
}
