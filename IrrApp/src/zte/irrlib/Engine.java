package zte.irrlib;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import zte.irrlib.core.Vector2i;
import zte.irrlib.scene.Scene;
import android.content.res.AssetManager;
import android.util.Log;

public class Engine{
	public static final String LogTag = "IrrEngine";
	
	public static Engine getInstance(){
		if (mUniInstance == null){
			mUniInstance = new Engine();
		}
		return mUniInstance;
	}
	
	public Scene getScene(){
		return mScene;
	}
	
	public Vector2i getRenderSize(){
		return new Vector2i(mWidth, mHeight);
	}
	
	public double getFPS(){
		//to be implemented.
		return 0;
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
	
	
	public String getResourceDir(){
		return mResourcePath;
	}
	
	public void setResourceDir(String path){
		mResourcePath = path;
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
	
	synchronized void create(){
		if (!mIsInit){
			nativeInit();
			mScene = Scene.getInstance(this);
			mIsInit = true;
		}
		mEventQueue.clear();
		mScene.init();
	}
	
	void resize(int w, int h){
		mWidth = w;
		mHeight = h;
		nativeResize(w, h);		
	}
	
	synchronized void finish(){
		if (mIsInit){
			mIsInit = false;
			//do not use device->drop().
		}
	}
	
	private static Engine mUniInstance = null;
	
	private boolean mIsInit;
	private String mResourcePath;
	private ArrayList<Event> mEventQueue;
	private int mWidth, mHeight;
	private Scene mScene;
	
	private Engine(){
		mEventQueue = new ArrayList<Event>();
	}

	private native void nativeInit();
	private native void nativeResize(int w, int h);
	//private native void nativeDrop(); <seems useless, and will cause error>
	private native double nativeGetFPS();
	
	//add in 1.24
	//it seems that device->run() does nothing, so ignore it 
	native void nativeBeginScene();
	native void nativeEndScene();
}