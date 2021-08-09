//
// Created by Administrator on 2021/8/9/009.
//

#include "Scene.h"

extern "C" void JNICALL
Java_com_ddq_opengl_MainActivity_init(JNIEnv *env,
                                      jobject
) {
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"%s\n","init");
    glClearColor(0.6f, 0.1f, 0.1f, 1.0f);
}

extern "C" void JNICALL
Java_com_ddq_opengl_MainActivity_onViewPortChange(JNIEnv *env,
                                                  jobject, jint width, jint height) {
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"%s%dx%d\n","nViewPortChange",width,height);
    glViewport(0, 0, width, height);
}

extern "C" JNICALL
void Java_com_ddq_opengl_MainActivity_drawFrame(JNIEnv *env,
                                                jobject) {
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"%s\n","drawFrame");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}