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

public class DemoRenderer implements Renderer {
	
	public static final String TAG = "DemoRenderer";

	public void onDrawFrame(Engine engine) {
		Scene scene = engine.getScene();
		
		scene.drawAllNodes();
		
		
		scene.drawText("fps: " + engine.getFPS(),
				new Vector2i(0, 0),
				new Color4i(0x90, 0x40, 0x90, 0xff));
	}

	public void onCreate(Engine engine) {
		origin = new Vector3d();
		back = new Vector3d(0, 0, 20);
		left = new Vector3d(-20, 0, 0);
		right = new Vector3d(20, 0, 0);
		
		engine.setResourceDir("/sdcard/irrmedia/");
		Scene scene = engine.getScene();
		scene.enableLighting(true);
		scene.addCameraSceneNode(new Vector3d(2, 10, -20), origin, true, null);
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
		/*model = scene.addAnimateMeshSceneNode("settings/settings.b3d", new Vector3d(0, 0, 10), null);
		model.setRotation(new Vector3d(-90, 0, 0), SceneNode.TRANS_ABSOLUTE);
		model.addRotationAnimator(new Vector3d(0, 0.5, 0), 0);
		node = scene.addTextNode("new", new Vector3d(0, 0, 1000), 10, null);
		node.setVisible(false);*/
		 
		cube = scene.addMeshSceneNode("models/axis.obj", origin, null);
		
		//cube = scene.addCubeSceneNode(origin, 10, null);
		cube.addRotationAnimator(new Vector3d(0,0.5,0.0));
		cube.setSmoothShade(true, 0);
		light = scene.addLightSceneNode(new Vector3d(-30,30,-30), 100, new Color3i(0x7f,0x7f,0x7f), null);
		light.downloadLightData();
		light.LightData.DiffuseColor = new Color3i(100, 100, 100);
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
	
	private Vector3d origin, back, left, right;
}
