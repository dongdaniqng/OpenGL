package com.ddq.opengl;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class DDQGlSurfaceView extends GLSurfaceView {
    private DDQRender render;

    public DDQGlSurfaceView(Context context) {
        super(context);
        init();
    }

    public DDQGlSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init(){
        setEGLContextClientVersion(2);
        render = new DDQRender();
        setRenderer(render);
    }
}
