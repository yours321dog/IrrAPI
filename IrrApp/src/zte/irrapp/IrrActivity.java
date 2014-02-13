
package zte.irrapp;

import zte.irrlib.Engine;
import zte.irrlib.IrrlichtView;
import zte.test.irrapp.R;
import android.app.Activity;
import android.os.Bundle;

public class IrrActivity extends Activity {
	IrrlichtView mDemo;
	protected String TAG = "IrrActivity";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.activity_irr);
		mDemo = (IrrlichtView)findViewById(R.id.irrview);
		mDemo.setRecommendEGLConfigChooser(4);
		mDemo.setEngineRenderer(new IrrlichtView.Renderer() {

			public void onDrawFrame(Engine engine) {
				
			}

			public void onCreate(Engine engine) {
				
			}

			public void onResize(Engine engine, int width, int height) {
				
			}
		});
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
