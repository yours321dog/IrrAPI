package zte.test.irrlib;

import java.util.concurrent.Semaphore;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL;

import zte.test.irrapp.WLog;
import android.graphics.SurfaceTexture;

public class IrrRenderThread extends Thread {
	private EglHelper mEglHelper;
	private Device mIrrDevice;
	private boolean mIsPause, mDone, mIsRunning, mIsChanged;
	private IrrTextureView.Renderer mRenderer;
	private SurfaceTexture mSurface;
	private int mWidth, mHeight;
	private boolean rendered;
	public final String LogTag = "IrrThread";
	
	private int [] mConfigSpec;
	
	private static final Semaphore mEglSemaphore = new Semaphore(1);
	
	public IrrRenderThread(){
		rendered = false;
	}
	
	public void setRenderer(IrrTextureView.Renderer renderer){
		mRenderer = renderer;
	}
	
	public void setConfigSpec(int []configSpec){
		mConfigSpec = configSpec;
	}
	
	public void onSurfaceAvailable(SurfaceTexture surface, int w, int h){
		mEglHelper = new EglHelper();
		mIrrDevice = Device.getInstance();
		mSurface = surface;
		mWidth = w;
		mHeight = h;
		mIsPause = false;
		mIsRunning = false;
		mIsChanged = false;
		mDone = false;
		
		if (mRenderer == null){
			//init a default renderer to avoid exception;
			WLog.i("no renderer installed, use default renderer");
			mRenderer = new IrrTextureView.Renderer() {
				@Override
				public void onSurfaceResize(Device device, int w, int h) {}
				@Override
				public void onSurfaceCreated(Device device, int w, int h) {}
				@Override
				public void drawIteration(Device device) {}
			};
		}
		
		start();
	}
	
	public synchronized void onSurfaceResized(SurfaceTexture surface, int w, int h){
		mSurface = surface;
		mWidth = w;
		mHeight = h;
		mIsChanged = true;
	}
	
	public synchronized void onSurfaceDestroyed(){
		if (mEglHelper != null){
			mDone = true;
		}
	}
	
	public synchronized void onPause(){
		mIsPause = true;
	}
	
	public synchronized void onResume(){
		mIsPause = false;
		notify();
	}
	
	public void run(){
        try {
            try {
            mEglSemaphore.acquire();
            } catch (InterruptedException e) {
                return;
            }
            guardedRun();
        } catch (InterruptedException e) {
           
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }
        finally {
            mEglSemaphore.release();
        }
	}
	
	private void guardedRun() throws InterruptedException{
		boolean needStart = true;
		
		while (!mDone){
			synchronized(this){
				if (mIsPause && mIsRunning){
					mEglHelper.finish();
					WLog.i("??");
					mIrrDevice.finish();
					WLog.i("!!");
					needStart = true;
					mIsRunning = false;
					wait();
				}
				
				if (needStart && !mIsRunning){
					needStart = false;
					WLog.i("restart?");
					mEglHelper.start(mConfigSpec);
					mEglHelper.createSurface(mSurface);
					mIrrDevice.init(mWidth, mHeight);
					mRenderer.onSurfaceCreated(mIrrDevice, mWidth, mHeight);
					mIsRunning = true;
				}
				
				if (mIsChanged){
					mEglHelper.createSurface(mSurface);
					mIrrDevice.resize(mWidth, mHeight);
					mRenderer.onSurfaceResize(mIrrDevice, mWidth, mHeight);
					mIsChanged = false;
				}
				if (mDone) break;
				
				if (mWidth > 0 && mHeight > 0 && !rendered){
					//mIrrDevice.drawIteration();
					//rendered = true;
					mIrrDevice.nativeBeginScene();
					mRenderer.drawIteration(mIrrDevice);
					mIrrDevice.nativeEndScene();
					mEglHelper.swap();
				}
			}
			//control FPS not completed yet
			sleep(20);
		}
		
		mEglHelper.finish();
	}
}



class EglHelper {
    EGL10 mEgl;
    EGLDisplay mEglDisplay;
    EGLSurface mEglSurface;
    EGLConfig mEglConfig;
    EGLContext mEglContext;
    
    public EglHelper() {

    }

    /**
     * Initialize EGL for a given configuration spec.
     * @param configSpec
     */
    public void start(int[] configSpec){
        /*
         * Get an EGL instance
         */
        mEgl = (EGL10) EGLContext.getEGL();

        /*
         * Get to the default display.
         */
        mEglDisplay = mEgl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);

        /*
         * We can now initialize EGL for that display
         */
        int[] version = new int[2];
        mEgl.eglInitialize(mEglDisplay, version);

        EGLConfig[] configs = new EGLConfig[1];
        int[] num_config = new int[1];
        mEgl.eglChooseConfig(mEglDisplay, configSpec, configs, 1,
                num_config);
        mEglConfig = configs[0];

        /*
        * Create an OpenGL ES context. This must be done only once, an
        * OpenGL context is a somewhat heavy object.
        */
        mEglContext = mEgl.eglCreateContext(mEglDisplay, mEglConfig,
                EGL10.EGL_NO_CONTEXT, null);

        mEglSurface = null;
    }

    
    /*
     * Create and return an OpenGL surface
     */
    public GL createSurface(Object surface) {
        /*
         *  The window size has changed, so we need to create a new
         *  surface.
         */
        if (mEglSurface != null) {

            /*
             * Unbind and destroy the old EGL surface, if
             * there is one.
             */
            mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE,
                    EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
            mEgl.eglDestroySurface(mEglDisplay, mEglSurface);
        }

        /*
         * Create an EGL surface we can render into.
         */
        mEglSurface = mEgl.eglCreateWindowSurface(mEglDisplay,
                mEglConfig, surface, null);

        /*
         * Before we can issue GL commands, we need to make sure
         * the context is current and bound to a surface.
         */
        mEgl.eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
                mEglContext);


        GL gl = mEglContext.getGL();
        
        return gl;
    }

    /**
     * Display the current render surface.
     * @return false if the context has been lost.
     */
    public boolean swap() {
        mEgl.eglSwapBuffers(mEglDisplay, mEglSurface);

        /*
         * Always check for EGL_CONTEXT_LOST, which means the context
         * and all associated data were lost (For instance because
         * the device went to sleep). We need to sleep until we
         * get a new surface.
         */
        return mEgl.eglGetError() != EGL11.EGL_CONTEXT_LOST;
    }

    public void finish() {
        if (mEglSurface != null) {
            mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE,
                    EGL10.EGL_NO_SURFACE,
                    EGL10.EGL_NO_CONTEXT);
            mEgl.eglDestroySurface(mEglDisplay, mEglSurface);
            mEglSurface = null;
        }
        if (mEglContext != null) {
            mEgl.eglDestroyContext(mEglDisplay, mEglContext);
            mEglContext = null;
        }
        if (mEglDisplay != null) {
            mEgl.eglTerminate(mEglDisplay);
            mEglDisplay = null;
        }
    }

    public int getError () {
    	return mEgl.eglGetError();
    }
}
