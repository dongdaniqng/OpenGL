//
// Created by Administrator on 2021/8/9/009.
//

#include "Scene.h"
#include "Utils.h"

static AAssetManager *assetManager = nullptr;

GLuint vbo;
GLuint program;
GLint pLocation;
GLint mLocation;
GLint vLocation;
GLint position;

extern "C" void JNICALL
Java_com_ddq_opengl_MainActivity_init(JNIEnv *env,
                                      jobject,
                                      jobject am
) {
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s\n", "init");
    glClearColor(0.6f, 0.1f, 0.1f, 1.0f);
    assetManager = AAssetManager_fromJava(env, am);

    Vertex vertexs[3]; //cpu -> gpu
    vertexs[0].aPosition[0] = -0.5f;
    vertexs[0].aPosition[1] = -0.5f;
    vertexs[0].aPosition[2] = -2.0f;
    vertexs[0].aPosition[3] = 1.0f;

    vertexs[1].aPosition[0] = 0.5f;
    vertexs[1].aPosition[1] = -0.5f;
    vertexs[1].aPosition[2] = -2.0f;
    vertexs[1].aPosition[3] = 1.0f;

    vertexs[2].aPosition[0] = 0.0f;
    vertexs[2].aPosition[1] = 0.5f;
    vertexs[2].aPosition[2] = -2.0f;
    vertexs[2].aPosition[3] = 1.0f;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 3, vertexs);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    program = createStandardProgram("glsl/t1.vx.glsl", "glsl/t1.fg.glsl");
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "program id:%i \n", program);

    mLocation = glGetUniformLocation(program, "u_ModelMatrix");
    vLocation = glGetUniformLocation(program, "u_ViewMatrix");
    pLocation = glGetUniformLocation(program, "u_ProjectionMatrix");
    position = glGetAttribLocation(program, "position");

    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,
                        "\nu_ModelMatrix:%i \n u_ViewMatrix:%i \n u_ProjectionMatrix:%i \n position:%i \n",
                        mLocation, vLocation, pLocation, position);


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
    //__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s%f\n", "drawFrame:", getFrameTime());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


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