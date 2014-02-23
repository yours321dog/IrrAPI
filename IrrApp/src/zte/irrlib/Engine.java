package zte.irrlib;

import java.util.ArrayList;

import zte.irrlib.core.Color3i;
import zte.irrlib.core.Color4i;
import zte.irrlib.core.Rect4i;
import zte.irrlib.core.Vector2i;
import zte.irrlib.core.Vector3d;
import zte.irrlib.scene.Scene;
import android.util.Log;

//向上负责与视图类的通信，向下负责管理引擎的java和native层
public class Engine{
	public static final String TAG = "IrrEngine";
	
	public static Engine getInstance(){
		if (mUniInstance == null){
			mUniInstance = new Engine();
		}
		return mUniInstance;
	}
	
	public void setResourceDir(String path){
		mScene.setResourceDir(path);
	}
	
	public void setRenderer(Renderer renderer){
		mRenderer = renderer;
	}
	
	public void setRenderType(int type){
		mRenderType = type;
	}
	
	public void setEvent(Event event){
		mEvent = event;
	}
	
	public Scene getScene(){
		return mScene;
	}
	
	public String getResourceDir(){
		return mScene.getResourceDir();
	}
	
	public Vector2i getRenderSize(){
		return mScene.getRenderSize();
	}
	
	public Event getEvent(){
		return mEvent;
	}
	
	public double getFPS(){
		//to be implemented.
		return nativeGetFPS();
	}
	
	/*public synchronized boolean queueEvent(Event event){
		//max event number is 255;
		if (mEventQueue.size() < 255){
			mEventQueue.add(event);
			return true;
		}
		else {
			Log.w(TAG, "event queue is full.");
			return false;
		}
	}*/
	
	/*public synchronized Event getEvent(){
		if (mEventQueue.size() <= 0){
			return null;
		}
		else{
			return mEventQueue.remove(0);
		}
	}*/
	
	public synchronized void onDestroy(){
		if (mIsInit) javaClear();
		if (nativeIsInit()) nativeClear();
		Log.d(TAG, "OnDestroy");
	}
	
	public synchronized void onSurfaceCreated(){
		if (!checkState()){
			mScene = Scene.getInstance(this);
			
			//do some clean
			if (mIsInit) javaClear();
			if (nativeIsInit()) nativeClear();
			//re-initialize
			nativeInit(mRenderType, new Vector3d(), new Color4i(), new Color3i(), new Rect4i());
			JavaInit();
			mRenderer.onCreate(this);
			Log.d(TAG, "OnSurfaceCreated if");
		}
		Log.d(TAG, "OnSurfaceCreated");
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
	
	private Event mEvent;
	private Scene mScene;
	private Renderer mRenderer;
	private int mRenderType = EGL10Ext.EGL_OPENGL_ES1_BIT;
	private ArrayList<Event> mEventQueue;
	
	private boolean mIsInit;
	
	private native int nativeInit(int rendertype, Vector3d vec, Color4i color4, Color3i color3, Rect4i rect);
	private native void nativeClear();
	private native boolean nativeIsInit();
	private native void nativeResize(int w, int h);
	private native double nativeGetFPS();
	
	native void nativeBeginScene();
	native void nativeEndScene();
	
	public interface Renderer {
		void onDrawFrame(Engine engine);
		void onCreate(Engine engine);
		void onResize(Engine engine, int width, int height);
	}
}