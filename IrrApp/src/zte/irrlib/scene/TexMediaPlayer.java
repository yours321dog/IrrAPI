package zte.irrlib.scene;

import java.io.IOException;

import zte.irrlib.Utils;
import android.graphics.SurfaceTexture;
import android.media.MediaPlayer;
import android.view.Surface;

public class TexMediaPlayer extends MediaPlayer
	implements SurfaceTexture.OnFrameAvailableListener{
	
	public TexMediaPlayer(Scene sc){
		super();
		mScene = sc;
	}
	
	public TexMediaPlayer(Scene sc, int texId){
		this(sc);
		setTexId(texId);
	}
	
	public static final String TAG = "TexMediaPlayer";
	
	public synchronized void update(){
		if ((mSurfaceTex!=null) && mNewFrame){
			mSurfaceTex.updateTexImage();
			mNewFrame = false;
		}
	}
	
	@Override
	public void setDataSource(String path)
			throws IllegalArgumentException, SecurityException,
			IllegalStateException, IOException{
		
		if (Utils.isAbsolutePath(path)){
			super.setDataSource(path);
		}
		else{
			super.setDataSource(mScene.getResourceDir()+path);
		}
	}
	
	void setTexId(int id){
		mSurfaceTex = new SurfaceTexture(id);
		mSurfaceTex.setOnFrameAvailableListener(this);
		setSurface(new Surface(mSurfaceTex));
	}

	private SurfaceTexture mSurfaceTex;
	private boolean mNewFrame;
	private Scene mScene;
	
	//! SurfaceTexture.OnFrameAvailableListener
	public synchronized void onFrameAvailable(SurfaceTexture sf) {
		mNewFrame = true;
	}
}
