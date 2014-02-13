
package zte.irrapp;

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
		mDemo.setEngineRenderer(new DemoRenderer());
	}
	
	@Override
	protected void onResume(){
		super.onResume();
		if (mDemo != null){
			mDemo.onResume();
			WLog.i("onResume.");
		}
	}
	
	@Override
	protected void onPause(){
		if (mDemo != null){
			mDemo.onPause();
			WLog.i("onPause.");
		}
		super.onPause();
	}
}
