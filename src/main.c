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

    t_geometry_pass g_pass;
    geometry_pass_init(&g_pass);

    t_lighting_pass l_pass;
    lighting_pass_init(&l_pass);
	l_pass.g_pass = &g_pass;

	t_vao cube_vao, quad_vao;
	vao_init_cube(&cube_vao);
    vao_init_quad(&quad_vao);

	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	// glFrontFace(GL_CW);

// 렌더링 루프
	// 각 VAO들이 기하 패스를 모두 거치게끔 하여 g-buffer에 데이터를 쌓은 후 라이팅 패스를 거치도록한다
	// geometry pass는 각 오브젝트 별로 한번씩 수행, 최상단 픽셀에 대한 정보가 모아졌으니 lighting pass는 한번만 수행
    while (!glfwWindowShouldClose(window))
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0, 0.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 지오메트리 패스
		glEnable(GL_DEPTH_TEST);
		geometry_pass_draw(&g_pass, &cube_vao, true);

// 라이팅 패스
		// lighting_pass_draw(&l_pass, &quad_vao);

        // 프레임 스와핑 및 이벤트 처리
		glfwSwapBuffers(window);
		glfwPollEvents();
    }
	glfwTerminate();
}
