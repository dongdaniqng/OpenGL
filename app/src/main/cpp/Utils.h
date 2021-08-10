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