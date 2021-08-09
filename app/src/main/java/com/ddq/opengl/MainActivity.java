package com.ddq.opengl;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    private DDQGlSurfaceView glSurfaceView;
    private static MainActivity self;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        self = this;
        glSurfaceView = findViewById(R.id.gl_surface_view);
    }

    public static MainActivity getInstance() {
        return self;
    }

    public native void init();
    public native void onViewPortChange(int width,int height);
    public native void drawFrame();
}


