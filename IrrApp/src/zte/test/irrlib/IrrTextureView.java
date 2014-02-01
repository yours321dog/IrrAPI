package zte.test.irrlib;

import javax.microedition.khronos.egl.EGL10;

import zte.test.irrapp.WLog;
import android.content.Context;
import android.graphics.SurfaceTexture;
import android.view.KeyEvent;
import android.view.TextureView;

public class IrrTextureView extends TextureView implements TextureView.SurfaceTextureListener{
	protected IrrRenderThread mRenderThread;
	protected Renderer mRenderer;
	protected Device mDevice;
	
	protected int [] mConfigSpec = {
        EGL10.EGL_RED_SIZE, 5,
        EGL10.EGL_GREEN_SIZE, 6,
        EGL10.EGL_BLUE_SIZE, 5,
        EGL10.EGL_DEPTH_SIZE, 16,
        // Requires that setEGLContextClientVersion(2) is called on the view.
        //EGL10.EGL_RENDERABLE_TYPE, 4 /* EGL_OPENGL_ES2_BIT */,
        EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
        EGL10.EGL_SAMPLES, 4,
        EGL10.EGL_NONE
	};
	
	public IrrTextureView(Context context) {
		super(context);
		mDevice = Device.getInstance();
		setSurfaceTextureListener(this);
	}
	
	public void setRenderer(Renderer rend){
		mRenderer = rend;
		
		if (mRenderThread != null){
			mRenderThread.setRenderer(mRenderer);
		}
	}
	
	public void setConfigSpec(int []configSpec){
		mConfigSpec = configSpec;
		
		if (mRenderThread != null){
			mRenderThread.setConfigSpec(configSpec);
		}
	}
	
	public void onPause(){
		if (mRenderThread != null) 
			mRenderThread.onPause();
		
		WLog.i("pause!"); 
	}
	
	public void onResume(){
		if (mRenderThread != null) 
			mRenderThread.onResume();
		
		WLog.i("resume!");
	}

	@Override
	public void onSurfaceTextureAvailable(SurfaceTexture surface, int w,
			int h) {
		WLog.i("Surface Created?");
		if (mRenderThread == null){
			mRenderThread = new IrrRenderThread();
		}
		if (mRenderer != null){
			mRenderThread.setRenderer(mRenderer);
		}
		
		mRenderThread.setConfigSpec(mConfigSpec);		
		mRenderThread.onSurfaceAvailable(surface, w, h);
	}

	@Override
	public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
		WLog.i("SurfaceDestroy?");
		if (mRenderThread != null){
			mRenderThread.onSurfaceDestroyed();
		}
		mRenderer = null;
		
		return false;
	}

	@Override
	public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int w,
			int h) {
		WLog.i("SurfaceResize?");
		if (mRenderThread != null){
			mRenderThread.onSurfaceResized(surface, w, h);
		}
	}

	@Override
	public void onSurfaceTextureUpdated(SurfaceTexture surface) {
		
	}

	public interface Renderer{
		void onSurfaceCreated(Device device, int w, int h);
		void drawIteration(Device device);
		void onSurfaceResize(Device device, int w, int h);
	}
	
	//事件响应范例, 1.25添加
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event){
		
		Event myEvent = new Event();
		myEvent.EventType = Event.KEY_DOWN_EVENT;
		myEvent.KeyCode = keyCode;
		myEvent.KeyEvent = event;
		//这里到底应该返回什么？
		return mDevice.queueEvent(myEvent);
	}
}
