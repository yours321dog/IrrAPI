package zte.irrapp;

import zte.irrlib.Engine;
import zte.irrlib.Engine.Renderer;
import zte.irrlib.core.Color4i;
import zte.irrlib.core.Vector2i;
import zte.irrlib.core.Vector3d;
import zte.irrlib.scene.MeshSceneNode;
import zte.irrlib.scene.Scene;
import zte.irrlib.scene.SceneNode;

public class DemoRenderer implements Renderer {

	@Override
	public void onDrawFrame(Engine engine) {
		Scene scene = engine.getScene();
		cube.setRotation(new Vector3d(1, 0.2, 0), SceneNode.RELATIVE_TRANSFORM);
		scene.drawAllNodes();
		
	}

	@Override
	public void onCreate(Engine engine) {
		Scene scene = engine.getScene();
		cube = scene.addCubeSceneNode(new Vector3d(0, 0, 0), 10, null);
		cube.setPosition(new Vector3d(0,0,20));
	}

	@Override
	public void onResize(Engine engine, int width, int height) {

	}

	private MeshSceneNode cube;
}
