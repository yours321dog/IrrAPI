package zte.irrlib.scene;

import android.graphics.SurfaceTexture;
import android.media.MediaPlayer;
import android.view.Surface;

public class TexMediaPlayer extends MediaPlayer
	implements SurfaceTexture.OnFrameAvailableListener{
	
	public TexMediaPlayer(){
		super();
	}
	
	public TexMediaPlayer(int texId){
		super();
		setTexId(texId);
	}
	
	public static final String TAG = "TexMediaPlayer";
	
	public synchronized void update(){
		if ((mSurfaceTex!=null) && mNewFrame){
			mSurfaceTex.updateTexImage();
			mNewFrame = false;
		}
	}
	
	void setTexId(int id){
		mSurfaceTex = new SurfaceTexture(id);
		mSurfaceTex.setOnFrameAvailableListener(this);
		setSurface(new Surface(mSurfaceTex));
	}

	private SurfaceTexture mSurfaceTex;
	private boolean mNewFrame;
	
	//! SurfaceTexture.OnFrameAvailableListener
	public synchronized void onFrameAvailable(SurfaceTexture sf) {
		mNewFrame = true;
	}
}
