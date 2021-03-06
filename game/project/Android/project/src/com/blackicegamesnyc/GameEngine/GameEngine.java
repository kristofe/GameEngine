package com.blackicegamesnyc.GameEngine;

import java.io.IOException;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;

public class GameEngine extends Activity {
	private GLSurfaceView mGLView;

	static {
		System.loadLibrary("NativeGameEngine");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mGLView = new GameEngineGLSurface(this);
		setContentView(mGLView);
		
    //Force landscape
    setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	}
    
	@Override
	protected void onPause() {
		super.onPause();
		mGLView.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		mGLView.onResume();
	}
}

class GameEngineGLSurface extends GLSurfaceView {
	GameEngineRenderer mRenderer;

	private static native void nativePause();
	
	public GameEngineGLSurface(Context context) {
		super(context);
		mRenderer = new GameEngineRenderer(context);
		setRenderer(mRenderer);
	}

	public boolean onTouchEvent(final MotionEvent event) {
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			nativePause();
		}
		return true;
	}
}

class GameEngineRenderer implements GLSurfaceView.Renderer {
	private Context context;
	public GameEngineRenderer (Context context) {
		this.context = context;
	}
	
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		// return apk file path (or null on error)
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = context.getPackageManager();
		try {
	    appInfo = packMgmr.getApplicationInfo("com.blackicegamesnyc.GameEngine", 0);
    } catch (NameNotFoundException e) {
	    e.printStackTrace();
	    throw new RuntimeException("Unable to locate assets, aborting...");
    }
		apkFilePath = appInfo.sourceDir;
		nativeInit(apkFilePath);
	}

	public void onSurfaceChanged(GL10 gl, int w, int h) {
		// gl.glViewport(0, 0, w, h);
		nativeResize(w, h);
	}

	public void onDrawFrame(GL10 gl) {
		nativeRender();
	}

	private static native void nativeInit(String apkPath);

	private static native void nativeResize(int w, int h);

	private static native void nativeRender();
}
