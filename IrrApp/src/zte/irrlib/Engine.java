package zte.irrlib;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import zte.irrapp.WLog;
import zte.irrlib.core.Vector2i;
import zte.irrlib.scene.Scene;
import android.content.res.AssetManager;
import android.util.Log;

//向上负责与视图类的通信，向下负责管理引擎的java和native层
public class Engine{
	public static final String LogTag = "IrrEngine";
	
	public static Engine getInstance(){
		if (mUniInstance == null){
			mUniInstance = new Engine();
		}
		return mUniInstance;
	}
	
	public void setResourceDir(String path){
		mResourcePath = path;
	}
	
	public void setRenderer(Renderer renderer){
		mRenderer = renderer;
	}
	
	public void setRenderType(int type){
		mRenderType = type;
	}
	
	public Scene getScene(){
		return mScene;
	}
	
	public String getResourceDir(){
		return mResourcePath;
	}
	
	public Vector2i getRenderSize(){
		return mScene.getRenderSize();
	}
	
	public double getFPS(){
		//to be implemented.
		return nativeGetFPS();
	}
	
	public synchronized boolean queueEvent(Event event){
		//max event number is 255;
		if (mEventQueue.size() < 255){
			mEventQueue.add(event);
			return true;
		}
		else {
			Log.w(LogTag, "event queue is full.");
			return false;
		}
	}
	
	public synchronized Event getEvent(){
		if (mEventQueue.size() <= 0){
			return null;
		}
		else{
			return mEventQueue.remove(0);
		}
	}
	
	public void copyAssetsToNative(AssetManager assetManager, boolean isMandatory) throws IOException{
		if (!isMandatory){
			File checkFile = new File(getResourceDir());
			if (checkFile.exists() && checkFile.isDirectory()){
				return;
			}
		}
		String[] fileList = assetManager.list("");
		for(String file:fileList){
			InputStream input = assetManager.open(file);
			OutputStream output =
					new FileOutputStream(getResourceDir() + "/" + file);
			byte[] buffer = new byte[4096];
			int length;
			while ((length = input.read(buffer)) > 0){
				output.write(buffer, 0, length);
			}
			output.flush();
			output.close();
			input.close();
		}
	}
	
	public synchronized void onDestroy(){
		if (mIsInit) javaClear();
		if (nativeIsInit()) nativeClear();
		getScene().getMediaPlayer().release();
	}
	
	public synchronized void onSurfaceCreated(){
		if (!checkState()){
			mScene = Scene.getInstance(this);
			
			//do some clean
			if (mIsInit) javaClear();
			if (nativeIsInit()) nativeClear();
			//re-initialize
			nativeInit(mRenderType);
			JavaInit();
			mRenderer.onCreate(this);
		}
	}
	
	public void onSurfaceChanged(int width, int height){
		nativeResize(width, height);
		mScene.onResize(width, height);
		mRenderer.onResize(this, width, height);
	}
	
	public void onDrawFrame(){
		nativeBeginScene();
		mRenderer.onDrawFrame(this);
		nativeEndScene();
	}
	
	public void startMediaPlayer(){
		getScene().getMediaPlayer().start();
	}
	
	private int JavaInit(){
		mScene.init();
		mIsInit = true; 
		return 0;
	}
	
	private void javaClear(){
		if (mEventQueue != null) mEventQueue.clear();
		mScene.javaClear();
		
		mIsInit = false; 
	}
	
	//当且仅当native引擎和Java接口都已经被初始化时返回true
	private boolean checkState(){
		return (mIsInit && nativeIsInit());
	}
	
	private Engine(){
		mEventQueue = new ArrayList<Event>();
	}
	
	private static Engine mUniInstance;
	
	private Scene mScene;
	private Renderer mRenderer;
	private int mRenderType = EGL10Ext.EGL_OPENGL_ES1_BIT;
	private ArrayList<Event> mEventQueue;
	
	private String mResourcePath;
	private boolean mIsInit;
	


	private native int nativeInit(int rendertype);
	private native void nativeClear();
	private native boolean nativeIsInit();
	
	private native void nativeResize(int w, int h);
	//private native void nativeDrop();  ***  seems useless, and will cause error
	private native double nativeGetFPS();
	native void nativeBeginScene();
	native void nativeEndScene();
	
	public interface Renderer {
		void onDrawFrame(Engine engine);
		void onCreate(Engine engine);
		void onResize(Engine engine, int width, int height);
	}
}