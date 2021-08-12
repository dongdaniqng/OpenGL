//
// Created by Administrator on 2021/8/10/010.
//


#pragma once

#include "AllHeader.h"

struct Vertex{
    // x y x w
    float aPosition[4];
};

char * loadFileContent(char * path, int & fileSize);

float getFrameTime();

GLuint compileShader(GLenum gl_type, char * content);

GLuint createProgram(GLuint vx,GLuint fg);

GLuint createStandardProgram(char *vx_path, char *fg_path);