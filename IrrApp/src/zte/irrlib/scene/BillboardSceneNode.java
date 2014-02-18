package zte.irrlib.scene;

import zte.irrlib.core.Color4i;

public class BillboardSceneNode extends MeshSceneNode{
	
	public BillboardSceneNode(){
		super();
		mNodeType = TYPE_BILLBOARD;
	}
	
	public void setColor(Color4i colorFront, Color4i colorBack){
		nativeSetColor(colorFront.r(), colorFront.g(), colorFront.b(), colorFront.a(),
				colorBack.r(), colorBack.g(), colorBack.b(), colorBack.a(), getId());
	}
	
	private native void nativeSetColor(int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int id);
}
