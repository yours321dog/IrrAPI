
package zte.test.irrapp;

import java.io.IOException;

import zte.test.irrlib.Device;
import zte.test.irrlib.IrrTextureView;
import zte.test.irrlib.NodeFactory;
import zte.test.irrlib.Scene;
import zte.test.irrlib.Vector3d;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class IrrActivity extends Activity {
	IrrTextureView mDemo;
	protected String LogTag = "IrrActivity";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		WLog.i("onCreated");
		super.onCreate(savedInstanceState);
		mDemo = new IrrTextureView(this);

		mDemo.setRenderer(new IrrTextureView.Renderer() {

			@Override
			public void onSurfaceResize(Device device, int w, int h) {
				
			}
			
			@Override
			public void onSurfaceCreated(Device device, int w, int h) {
				NodeFactory nf = device.getNodeFactory();
				nf.addCubeSceneNode(new Vector3d(0, 0, -100), new Vector3d(10, 10, 20), null);
			}
			
			@Override
			public void drawIteration(Device device) {
				Scene cv = device.getScene();
				cv.drawAllNodes();
			}
		});
		
		//目前材质的本地调用没什么好办法，暂时还是用拷贝
		Device irrDev = Device.getInstance();
		irrDev.setNativeAssetsPath(getFilesDir().getAbsolutePath() + "/IrrAssets");
		try{
			irrDev.copyAssetsToNative(getAssets(), false);
		} catch (IOException e){
			Log.e(LogTag, "copy assets error!");
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
}
