package zte.irrapp;

import zte.irrlib.Engine;
import zte.irrlib.IrrlichtView.Renderer;
import zte.irrlib.core.Color4i;
import zte.irrlib.core.Vector2i;
import zte.irrlib.core.Vector3d;
import zte.irrlib.scene.Scene;

public class DemoRenderer implements Renderer {

	@Override
	public void onDrawFrame(Engine engine) {
		Scene scene = engine.getScene();
		scene.drawAllNodes();
		scene.drawText("new", new Vector2i(10,10), 30, new Color4i(1,0,0,1));
	}

	@Override
	public void onCreate(Engine engine) {
		Scene scene = engine.getScene();
		scene.addCubeSceneNode(new Vector3d(0, 0, 0), new Vector3d(10, 10, 10), null);
	}

	@Override
	public void onResize(Engine engine, int width, int height) {

	}

}
