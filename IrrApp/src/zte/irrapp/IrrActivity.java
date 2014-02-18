
package zte.irrapp;

import zte.irrlib.IrrlichtView;
import zte.irrlib.scene.TextureMediaPlayer;
import zte.test.irrapp.R;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class IrrActivity extends Activity {
	IrrlichtView mDemo;
	Button button;
	protected String TAG = "IrrActivity";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_irr);
		mDemo = (IrrlichtView)findViewById(R.id.irrview);
		mDemo.setRecommendEGLConfigChooser(8);
		mDemo.setEngineRenderer(new DemoRenderer());
		
		button = (Button)findViewById(R.id.button_start);
		button.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View view) {
				mDemo.startMediaPlayer();
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
	
	/*@Override
	protected void onDestroy(){
		if (mDemo != null){
			mDemo.onDestroy();
		}
		WLog.i("onDestroy");
		super.onDestroy();
	}*/
}
