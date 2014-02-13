
package zte.irrapp;

import java.io.IOException;

import zte.irrlib.Engine;
import zte.irrlib.IrrlichtView;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class IrrActivity extends Activity {
	IrrlichtView mDemo;
	protected String TAG = "IrrActivity";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		WLog.i("onCreated");
		super.onCreate(savedInstanceState);
		mDemo = new IrrlichtView(this);
		mDemo.setRecommendEGLConfigChooser(4);
		mDemo.setEngineRenderer(new IrrlichtView.Renderer() {

			public void onDrawFrame(Engine engine) {
				
			}

			public void onCreate(Engine engine) {
				
			}

			public void onResize(Engine engine, int width, int height) {
				
			}
		});
		
		//目前材质的本地调用没什么好办法，暂时还是用拷贝
		Engine engine = Engine.getInstance();
		engine.setResourceDir(getFilesDir().getAbsolutePath() + "/IrrAssets");
		try{
			engine.copyAssetsToNative(getAssets(), false);
		} catch (IOException e){
			Log.e(TAG, "copy assets error!");
			e.printStackTrace();
		}
		
		setContentView(mDemo);
		//setContentView(R.layout.activity_irr);
	}
	
	@Override
	protected void onResume(){
		super.onResume();
		if (mDemo != null){
			mDemo.onResume();
		}
	}
	
	@Override
	protected void onPause(){
		if (mDemo != null){
			mDemo.onPause();
		}
		super.onPause();
	}
	
	@Override
	protected void onDestroy(){
		if (mDemo != null){
			mDemo.onDestroy();
		}
		super.onDestroy();
	}
}
