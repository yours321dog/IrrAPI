package zte.irrlib.scene;

import java.io.File;

import zte.irrapp.WLog;
import android.graphics.SurfaceTexture;
import android.media.MediaPlayer;
import android.view.Surface;

public class TextureMediaPlayer {
	
	public TextureMediaPlayer(){
		mMediaPlayer = new MediaPlayer();
		
        mMediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {

            public void onPrepared(MediaPlayer mediaPlayer) {
                int width = mediaPlayer.getVideoWidth();
                int height = mediaPlayer.getVideoHeight();
                WLog.i("onPrepared width=" + width + ", height=" + height);
                
                mediaPlayer.start();
            }

        });
	}
	
	public void setDataSource(String path){
		mDataSource = path;
	}
	
	void setTexId(int id){
		mTexId = id;
		msf = new SurfaceTexture(id);
	}
	
	public void update(){
		msf.updateTexImage();
	}
	
	public void start(){
		//File f = Environment.getExternalStorageDirectory();
		//"/storage/extSdCard/irrmedia/media.ts"
		if (!fileIsExists(mDataSource)){
			WLog.i("can not find media");
			return;
		}
		
		try {
			mMediaPlayer.setDataSource(mDataSource);
		} catch (Exception e) {
			e.printStackTrace();
		}
		mMediaPlayer.setSurface(new Surface(msf));
		mMediaPlayer.prepareAsync();
	}
	
	public void stop(){
		mMediaPlayer.stop();
	}
	
	public void release(){
		mMediaPlayer.release();
	}
	
	protected boolean fileIsExists(String path){
        try{
                File f=new File(path);
                if(!f.exists()){
                        return false;
                }
                
        }catch (Exception e) {
                // TODO: handle exception
                return false;
        }
        return true;
	}
	
	private MediaPlayer mMediaPlayer;
	private int mTexId;
	private SurfaceTexture msf;
	private String mDataSource;
}
