package com.ddq.opengl;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class DDQRender implements GLSurfaceView.Renderer {

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        MainActivity.getInstance().init(MainActivity.getInstance().getAssets());
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {
        MainActivity.getInstance().onViewPortChange(i, i1);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        MainActivity.getInstance().drawFrame();
    }
}
