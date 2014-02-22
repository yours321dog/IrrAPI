package zte.irrlib;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.res.AssetManager;
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
	
	public static boolean isAbsolutePath(String path){
		return (path.charAt(0) == '/');
	}
	
	/*public void copyAssetsToNative(AssetManager assetManager, boolean isMandatory) throws IOException{
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
	}*/
}
