package zte.irrapp;

import zte.irrlib.Engine;
import zte.irrlib.Engine.Renderer;
import zte.irrlib.core.Color3i;
import zte.irrlib.core.Color4i;
import zte.irrlib.core.Vector2i;
import zte.irrlib.core.Vector3d;
import zte.irrlib.scene.LightSceneNode;
import zte.irrlib.scene.MeshSceneNode;
import zte.irrlib.scene.Scene;
import zte.irrlib.scene.SceneNode;
import zte.irrlib.scene.TexMediaPlayer;
import zte.irrlib.Event;

public class DemoRenderer implements Renderer {
	
	public static final String TAG = "DemoRenderer";

	public void onDrawFrame(Engine engine) {
		Scene scene = engine.getScene();
		
		scene.drawAllNodes();
		
		
		scene.drawText("fps: " + engine.getFPS() + " material: " + model.getMaterialCount(),
				new Vector2i(0, 0),
				new Color4i(0xff, 0x40, 0x90, 0xff));
		
		mEvent = engine.getEvent();
		if (mEvent != null){
			scene.drawText("touch: " + scene.getTouchedSceneNode((int)mEvent.motionEvent.getX(),
					(int)mEvent.motionEvent.getY(), model),
					new Vector2i(0, 20), 
					new Color4i(0xff, 0xff, 0xff, 0xff));
			
			scene.drawText("point: " + (int)mEvent.motionEvent.getX() + ":" +
					(int)mEvent.motionEvent.getY(),
					new Vector2i(0, 40), 
					new Color4i(0xff, 0xff, 0xff, 0xff));
		}
	}

	public void onCreate(Engine engine) {
		origin = new Vector3d();
		back = new Vector3d(0, 0, 20);
		left = new Vector3d(-20, 0, 0);
		right = new Vector3d(20, 0, 0);
		
		engine.setResourceDir("/storage/sdcard0/irrmedia/");
		Scene scene = engine.getScene();
		scene.setDefaultFontPath("bigfont.png");
		//scene.enableLighting(false);
		scene.addCameraSceneNode(new Vector3d(2, 10, -10), origin, true, null);
		/*mPlayer = scene.getMediaPlayer();
		
		try {
			mPlayer.setDataSource("/storage/extSdCard/irrmedia/media.ts");
		} catch (Exception e) {
			Log.e(TAG, "something wrong with mediaPlayer.");
			e.printStackTrace();
			mPlayer.release();
		} 
		
		try {
			mPlayer.prepare();
		} catch (IllegalStateException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}*/
		model = scene.addMeshSceneNode("mytv/mytv.b3d", origin, null);
		
		model.addRotationAnimator(new Vector3d(0,0.5,0.0));
		model.setSmoothShade(true, 0);
		model.setTouchable(true);
		model.setDiffuseColor(new Color4i(0xff, 0xff, 0, 0xff), 10);
		model.setRotation(new Vector3d(90, 0, 0), SceneNode.TRANS_ABSOLUTE);
		
		light = scene.addLightSceneNode(new Vector3d(-30,30,-30), 100, new Color3i(0x7f,0x7f,0x7f), null);
		
		light.LightData.AmbientColor = new Color3i(0, 0, 0);		
		light.LightData.DiffuseColor = new Color3i(0, 0, 0);
		light.LightData.SpecularColor = new Color3i(0, 0, 0);
		light.LightData.DiffuseColor = new Color3i(0x7f, 0x00, 0x7f);
		light.upLoadLightData();
	}
	
	public void onResize(Engine engine, int width, int height) {
		
	}
	
	public void startMediaPlayer(){
		mPlayer.start();
	}

	private SceneNode empty, node;
	private MeshSceneNode cube, model;
	private LightSceneNode light;
	private int count;
	private TexMediaPlayer mPlayer;
	private Event mEvent;
		
	private Vector3d origin, back, left, right;
}
