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
