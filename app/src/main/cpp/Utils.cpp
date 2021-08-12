//
// Created by Administrator on 2021/8/10/010.
//

#include "Utils.h"

float getFrameTime() {
    static long long lastTime = 0, currentTime = 0;
    timeval current;
    gettimeofday(&current, nullptr);
    currentTime = current.tv_sec * 1000 + current.tv_usec / 1000;
    long long frameTime = lastTime == 0 ? 0 : currentTime - lastTime;
    lastTime = currentTime;
    return frameTime * 1.0f;
}

GLuint compileShader(GLenum gl_type, char *content) {
    GLuint shader = glCreateShader(gl_type);
    glShaderSource(shader, 1, &content, nullptr);
    glCompileShader(shader);

    GLint compile_result = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);
    if (compile_result == GL_FALSE) {
        char szLog[1024] = {0};
        GLsizei logLth = 0;
        glGetShaderInfoLog(shader, 1024, &logLth, szLog);
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "compile shader fail->%s\ncode:%s", szLog,
                            content);
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}

GLuint createProgram(GLuint vx, GLuint fg) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vx);
    glAttachShader(program, fg);
    glLinkProgram(program);

    glDeleteShader(vx);
    glDeleteShader(fg);

    GLint result = GL_TRUE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        char szLog[1024] = {0};
        GLsizei logLth = 0;
        glGetShaderInfoLog(program, 1024, &logLth, szLog);
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "compile program fail:%s\n", szLog);
        glDeleteProgram(program);
        program = 0;
    }

    return program;
}

GLuint createStandardProgram(char *vx_path, char *fg_path) {
    int vx_file_size = 0;
    char *vx_content = loadFileContent(vx_path, vx_file_size);
    GLuint vx_shader = compileShader(GL_VERTEX_SHADER, vx_content);

    delete[] vx_content;

    int fg_file_size = 0;
    char *fg_content = loadFileContent(fg_path, fg_file_size);
    GLuint fg_shader = compileShader(GL_FRAGMENT_SHADER, fg_content);

    delete[] fg_content;

    return createProgram(vx_shader, fg_shader);
}
