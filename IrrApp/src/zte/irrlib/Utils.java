package zte.irrlib;

import java.io.File;

import android.util.Log;

public class Utils {

	public static final String TAG = "Untils";
	
	public static boolean fileIsExists(String path){
	    try{
            File f = new File(path);
            if(!f.exists()){
            	Log.w(TAG, "File not exists: " + path);
            	return false;
            }
	    }catch (Exception e) {
    		Log.w(TAG, "Path open failed: " + path);
            return false;
	    }
	    return true;
	}
}
