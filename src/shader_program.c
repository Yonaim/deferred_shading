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

static int compile_shader(GLuint *shader, char *source, int type)
{
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

int shader_program(GLuint *program, const char *vs_path, const char *fs_path)
{
    char *vs_source, *fs_source;
    GLuint vs_shader, fs_shader;

    if (vs_path)
    {
        read_file(vs_path, &vs_source);
        compile_shader(&vs_shader, vs_source, GL_VERTEX_SHADER);
    }
    read_file(fs_path, &fs_source);
    compile_shader(&fs_shader, fs_source, GL_FRAGMENT_SHADER);

    // 프로그램 생성, 링크
    *program = glCreateProgram();
    if (vs_path)
        glAttachShader(*program, vs_shader);
    glAttachShader(*program, fs_shader);
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

    if (vs_path)
        glDeleteShader(vs_shader);
    glDeleteShader(fs_shader);

    return (0);
}
