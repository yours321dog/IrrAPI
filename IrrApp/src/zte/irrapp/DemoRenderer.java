package zte.irrapp;

import zte.irrlib.Engine;
import zte.irrlib.Engine.Renderer;
import zte.irrlib.core.Color3i;
import zte.irrlib.core.Vector3d;
import zte.irrlib.scene.LightSceneNode;
import zte.irrlib.scene.MeshSceneNode;
import zte.irrlib.scene.Scene;
import zte.irrlib.scene.SceneNode;
import zte.irrlib.scene.TextureMediaPlayer;

public class DemoRenderer implements Renderer {

	public void onDrawFrame(Engine engine) {
		mPlayer.update();
		Scene scene = engine.getScene();
		cube.setRotation(new Vector3d(1, 0.2, 0), SceneNode.RELATIVE_TRANSFORM);
		scene.drawAllNodes();
		
		count = (count+1)%100;
		if (count == 0) WLog.i("fps: " + engine.getFPS());
	}

	public void onCreate(Engine engine) {		
		Scene scene = engine.getScene();
		
		mPlayer = scene.getMediaPlayer();
		
		cube = scene.addCubeSceneNode(new Vector3d(0, 0, 0), 10, null);
		cube.setPosition(new Vector3d(0,0,20));
		//cube.setTexture("/storage/extSdCard/irrmedia/test2.jpg", 0);
		scene.setMediaTexture(cube, 0);
		
		light = scene.addLightSceneNode(new Vector3d(30,30,-30), 100, new Color3i(0xff,0,0), null);
	}
	
	public void onResize(Engine engine, int width, int height) {
		
	}

	private MeshSceneNode cube;
	private LightSceneNode light;
	private int count;
	private TextureMediaPlayer mPlayer;
}
