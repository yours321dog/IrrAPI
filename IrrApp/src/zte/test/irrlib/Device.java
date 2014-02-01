package zte.test.irrlib;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import android.content.res.AssetManager;
import android.util.Log;

public class Device{
	static{
		System.loadLibrary("irrlicht");
	}
	
	private static Device mUniInstance = null;
	private String mNativeAssetsPath;
	private ArrayList<Event> mEventQueue;
	private int mWidth, mHeight;
	
	public static final String LogTag = "IrrDevice";
	
	private Device(){
		
	}
	
	public static Device getInstance(){
		if (mUniInstance == null){
			mUniInstance = new Device();
		}
		return mUniInstance;
	}
	
	public void setNativeAssetsPath(String path){
		mNativeAssetsPath = path;
	}
	
	public void copyAssetsToNative(AssetManager assetManager, boolean isMandatory) throws IOException{
		if (!isMandatory){
			File checkFile = new File(mNativeAssetsPath);
			if (checkFile.exists() && checkFile.isDirectory()){
				return;
			}
		}
		String[] fileList = assetManager.list("");
		for(String file:fileList){
			InputStream input = assetManager.open(file);
			OutputStream output =
					new FileOutputStream(mNativeAssetsPath + "/" + file);
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
	
	void init(int w, int h){
		//import GL, create devices
		mWidth = w;
		mHeight = h;
		nativeInit(w, h);
		
		//add default camera;
		getScene().ActiveCamera = 
				getNodeFactory().addCameraSceneNode(new Vector3d(0, 0, -10), new Vector3d(0, 0, 0), true, null);
	}
	
	void resize(int w, int h){
		mWidth = w;
		mHeight = h;
		nativeResize(w, h);		
	}
	
	void finish(){
		//drop devices
		nativeDrop();
	}
	
	//this is for jni test and should not be used in java API
	void drawIteration(){
		//draw a frame
		nativeDrawIteration();
	}
	
	public Scene getScene(){
		Scene cv = Scene.getInstance();
		cv.setIrrDevice(this);
		return cv;
	}
	
	public NodeFactory getNodeFactory(){
		NodeFactory nf = NodeFactory.getInstance();
		nf.setIrrDevice(this);
		return nf;
	}
	
	public String getPathPrefix(){
		return mNativeAssetsPath;
	}
	
	public Vector2i getRenderSize(){
		return new Vector2i(mWidth, mHeight);
	}
	
	public void setRendererClippingPlaneLimits(double nearClip, double farClip){
		nativeSetClipPlain(nearClip, farClip);
	}
	
	public double getFPS(){
		return nativeGetFPS();
	}
	
	public synchronized boolean queueEvent(Event event){
		//max event number is 255;
		if (mEventQueue.size() < 0xff){
			mEventQueue.add(event);
			return true;
		}
		else {
			Log.w(LogTag, "event queue is full.");
			return false;
		}
	}
	
	public synchronized Event getEvent(){
		if (mEventQueue.size() == 0){
			return null;
		}
		else{
			return mEventQueue.remove(0);
		}
	}
	
	private native void nativeInit(int w, int h);
	private native void nativeResize(int w, int h);
	private native void nativeDrop();
	private native void nativeDrawIteration();
	
	//add in 1.24
	//it seems that device->run() does nothing, so ignore it 
	native void nativeBeginScene();
	native void nativeEndScene();
	
	private native void nativeSetClipPlain(double nearClip, double farClip);
	private native double nativeGetFPS();
}