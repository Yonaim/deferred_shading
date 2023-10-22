#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

static int read_file(const char *path, char **out)
{
    FILE *file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "쉐이더 파일 열기에 실패했습니다.\n");
        return (-1);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file); // 파일 크기 확인
    fseek(file, 0, SEEK_SET);  // 파일 포인터를 시작점으로 되돌림

    *out = (char *)malloc(size + 1);
    fread(*out, sizeof(char), size, file);
    (*out)[size] = '\0';
    
    fclose(file);
    return (0);
}

int compile_shader(GLuint *shader, char *path, int type)
{
    char *source;
    read_file(path, &source);

    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, (const GLchar **)&source, NULL);
    glCompileShader(*shader);
    free(source);

    // 에러 체크
    GLint success;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(*shader, 512, NULL, infoLog);
        fprintf(stderr, "Vertex Shader 컴파일 에러: %s\n", infoLog);
        return (-1);
    }
    return (0);
}

int link_shaders(GLuint *program, GLuint vs, GLuint fs)
{
    *program = glCreateProgram();
    glAttachShader(*program, vs);
    glAttachShader(*program, fs);
    glLinkProgram(*program);

    // 에러 체크
    GLint success;
    glGetProgramiv(*program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(*program, 512, NULL, infoLog);
        fprintf(stderr, "쉐이더 프로그램 링크 에러: %s\n", infoLog);
        return (-1);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return (0);
}
