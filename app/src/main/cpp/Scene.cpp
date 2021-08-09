//
// Created by Administrator on 2021/8/9/009.
//

#include "Scene.h"
#include "Utils.h"

static AAssetManager *assetManager = nullptr;

char *loadFileContent(char *path, int &fileSize) {
    char *fileContent = nullptr;
    fileSize = 0;
    AAsset *asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);
    if (asset != nullptr) {
        fileSize = AAsset_getLength(asset);
        fileContent = new char[fileSize + 1];
        AAsset_read(asset, fileContent, fileSize);
        fileContent[fileSize] = 0;
        AAsset_close(asset);
    }
    return fileContent;
}

extern "C" void JNICALL
Java_com_ddq_opengl_MainActivity_init(JNIEnv *env,
                                      jobject,
                                      jobject am
) {
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s\n", "init");
    glClearColor(0.6f, 0.1f, 0.1f, 1.0f);
    assetManager = AAssetManager_fromJava(env, am);
    int fileSize = 0;
    char *content = loadFileContent("1/test.txt",fileSize);
    if (content != nullptr){
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s\n", content);
        delete content;
    }
}

extern "C" void JNICALL
Java_com_ddq_opengl_MainActivity_onViewPortChange(JNIEnv *env,
                                                  jobject, jint width, jint height) {
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s%dx%d\n", "nViewPortChange", width, height);
    glViewport(0, 0, width, height);
}

extern "C" JNICALL
void Java_com_ddq_opengl_MainActivity_drawFrame(JNIEnv *env,
                                                jobject) {
    //__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s\n", "drawFrame");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}