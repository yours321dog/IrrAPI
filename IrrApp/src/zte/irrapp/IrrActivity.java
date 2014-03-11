
package zte.irrapp;

import java.io.IOException;

import zte.irrlib.IrrlichtView;
import zte.irrlib.Utils;
import zte.test.irrapp.R;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class IrrActivity extends Activity {
	IrrlichtView mDemo;
	DemoRenderer mRenderer;
	Button button;
	protected String TAG = "IrrActivity";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Utils utils = new Utils();
		utils.UtilsInit(getAssets());
		setContentView(R.layout.activity_irr);
		mDemo = (IrrlichtView)findViewById(R.id.irrview);
		mDemo.setRecommendEGLConfigChooser(0);
		mDemo.enableGLES2(true);
		mRenderer = new DemoRenderer();
		mDemo.setEngineRenderer(mRenderer);
		
		button = (Button)findViewById(R.id.button_start);
		button.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View view) {
				mRenderer.startMediaPlayer();
			}
		});
		Log.d(TAG, "Activity onCreate.");
	}
	
	@Override
	protected void onResume(){
		super.onResume();
		if (mDemo != null){
			mDemo.onResume();
			Log.d(TAG, "Activity onResume.");
		}
	}
	
	@Override
	protected void onPause(){
		if (mDemo != null){
			mDemo.onPause();
		}
		super.onPause();
		Log.d(TAG, "Activity onPause.");
	}
	
	@Override
	protected void onDestroy(){
		super.onDestroy();
		Log.d(TAG, "Activity onDestroy");
	}
}
