package zte.irrlib;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import zte.irrapp.WLog;
import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;

public class IrrlichtView extends GLSurfaceView {

	public final String TAG = "IrrlichtView";
	
	public IrrlichtView(Context context) {		
		super(context);
		mEngine = Engine.getInstance();
	}
	
	public IrrlichtView(Context context, AttributeSet attrs) {
		super(context, attrs);
		mEngine = Engine.getInstance();
	}
	
	public void setRecommendEGLConfigChooser(int sampleLevel){
		setEGLConfigChooser(new RecommedEGLConfigChooser(mRenderType, sampleLevel));
	}
	
	public void enableGLES2(boolean flag){
		if (flag){
			mRenderType = EGL10Ext.EGL_OPENGL_ES2_BIT;
			setEGLContextClientVersion(2);
			mEngine.setRenderType(EGL10Ext.EGL_OPENGL_ES2_BIT);
		}
		else{
			mRenderType = EGL10Ext.EGL_OPENGL_ES1_BIT;
			setEGLContextClientVersion(1);
			mEngine.setRenderType(EGL10Ext.EGL_OPENGL_ES1_BIT);
		}
	}
	
	//method replacing GLSurfaceView.setRenderer.
	public void setEngineRenderer(Engine.Renderer renderer){
		/* source: http://developer.android.com/reference/android/opengl/GLSurfaceView.html
		 * 
		 * The following GLSurfaceView methods can only be called before setRenderer is called:
		 * 
		 * setEGLConfigChooser(boolean)
		 * setEGLConfigChooser(EGLConfigChooser)
		 * setEGLConfigChooser(int, int, int, int, int, int)
		 * 
		 * The following GLSurfaceView methods can only be called after setRenderer is called:
		 * 
		 * getRenderMode()
		 * onPause()
		 * onResume()
		 * queueEvent(Runnable)
		 * requestRender()
		 * setRenderMode(int)
		 */
		
		//since this method will automatically call GLSurfaceView.setRenderer, the same principle
		//above should be followed.
		
		mEngine.setRenderer(renderer);
		super.setRenderer(new GLSurfaceView.Renderer() {
			public void onSurfaceCreated(GL10 unused, EGLConfig config) {mEngine.onSurfaceCreated();}
			public void onSurfaceChanged(GL10 unused, int width, int height) {mEngine.onSurfaceChanged(width, height);}
			public void onDrawFrame(GL10 unused) {mEngine.onDrawFrame();}
		});
	}
	
	@Override
	protected void onDetachedFromWindow(){
		if (mEngine != null){
			mEngine.onDestroy();
		}
		super.onDetachedFromWindow();
	}
	
	/*public void onDestroy(){
		if (mEngine != null){
			mEngine.onDestroy();
		}
	}*/
	
	protected Activity getActivity(){
		return (Activity)getContext();
	}
	
	protected Engine mEngine;
	protected int mRenderType = EGL10Ext.EGL_OPENGL_ES1_BIT;
	
	static {
		System.loadLibrary("irrlicht");
	}
}

class RecommedEGLConfigChooser implements GLSurfaceView.EGLConfigChooser{
	
	public RecommedEGLConfigChooser(int renderType, int sampleLevel){
		mRenderType = renderType;
		mSampleLevel = sampleLevel;
	}
	
	public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
        int numConfigs[] = new int[1];
        int[] configSpec = null;
        
        if (mSampleLevel != 0){
	        // Try to find a normal multisample configuration first.
	        configSpec = new int[]{
	                EGL10.EGL_RED_SIZE, 5,
	                EGL10.EGL_GREEN_SIZE, 6,
	                EGL10.EGL_BLUE_SIZE, 5,
	                EGL10.EGL_DEPTH_SIZE, 16,
	                EGL10.EGL_RENDERABLE_TYPE, mRenderType,
	                EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
	                EGL10.EGL_SAMPLES, mSampleLevel,
	                EGL10.EGL_NONE
	        };
	        if (!egl.eglChooseConfig(display, configSpec, null, 0, numConfigs)) {
	            throw new IllegalArgumentException("eglChooseConfig failed: code 1");
	        }
	
	        if (numConfigs[0] <= 0) {
	            // No normal multisampling configuration was found. Try to create a
	            // multisampling configuration, for the nVidia Tegra2.
	            // See the EGL_NV_coverage_sample documentation.
	
	            configSpec = new int[]{
	                    EGL10.EGL_RED_SIZE, 5,
	                    EGL10.EGL_GREEN_SIZE, 6,
	                    EGL10.EGL_BLUE_SIZE, 5,
	                    EGL10.EGL_DEPTH_SIZE, 16,
	                    EGL10.EGL_RENDERABLE_TYPE, mRenderType,
	                    EGL10Ext.EGL_COVERAGE_BUFFERS_NV, 1 /* true */,
	                    EGL10Ext.EGL_COVERAGE_SAMPLES_NV, 2,  // always 5 in practice on tegra 2
	                    EGL10.EGL_NONE
	            };
	
	            if (!egl.eglChooseConfig(display, configSpec, null, 0, numConfigs)) {
	                throw new IllegalArgumentException("eglChooseConfig failed: code 2");
	            }
	        }
        }
        
        if (numConfigs[0] <= 0 || mSampleLevel == 0) {
	        //do without multisampling.
	        configSpec = new int[]{
	                EGL10.EGL_RED_SIZE, 5,
	                EGL10.EGL_GREEN_SIZE, 6,
	                EGL10.EGL_BLUE_SIZE, 5,
	                EGL10.EGL_DEPTH_SIZE, 16,
	                EGL10.EGL_RENDERABLE_TYPE, mRenderType,
	                EGL10.EGL_NONE
	        };

	        if (!egl.eglChooseConfig(display, configSpec, null, 0, numConfigs)) {
	            throw new IllegalArgumentException("eglChooseConfig failed: code 3");
	        }
	
	        if (numConfigs[0] <= 0) {
	          throw new IllegalArgumentException("No configs match configSpec!");
	        }
        }

        // Get all matching configurations.
        int numValue = numConfigs[0];
        EGLConfig[] configs = new EGLConfig[numValue];
        if (!egl.eglChooseConfig(display, configSpec, configs, numValue, numConfigs)) {
            throw new IllegalArgumentException("get eglChooseConfig data failed!");
        }

        // CAUTION! eglChooseConfigs returns configs with higher bit depth
        // first: Even though we asked for rgb565 configurations, rgb888
        // configurations are considered to be "better" and returned first.
        // You need to explicitly filter the data returned by eglChooseConfig!
        EGLConfig config = null;
        for (EGLConfig configIter: configs) {
            if (findConfigAttrib(egl, display, configIter, EGL10.EGL_RED_SIZE, -1) == 5) {
                config = configIter;
                break;
            }
        }
        if (config == null) {
            Log.w(toString(), "Did not find sane config, using first");
            config = configs[0];
        }
        return config;
	}
        
    private int findConfigAttrib(EGL10 egl, EGLDisplay display,
            EGLConfig config, int attribute, int defaultValue) {
        if (egl.eglGetConfigAttrib(display, config, attribute, tmp)) {
            return tmp[0];
        }
        return defaultValue;
    }
	
	protected int mRenderType = EGL10Ext.EGL_OPENGL_ES1_BIT;
	protected int mSampleLevel = 4;
	
	//create to avoid frequent new operation in findConfigAttrib
	private int tmp[] = new int[1];
}
//define some hard code in EGL10
class EGL10Ext{
	public static final int EGL_OPENGL_ES1_BIT = 0x00000001;
	public static final int EGL_OPENGL_ES2_BIT = 0x00000004;
	public static final int EGL_COVERAGE_BUFFERS_NV = 0x30E0;
	public static final int EGL_COVERAGE_SAMPLES_NV = 0x30E1;	
}
