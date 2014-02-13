package zte.irrlib.scene;

import zte.irrlib.core.Color4i;

public class BillboardSceneNode extends SceneNode implements SceneNode.MaterialOwner{
	
	public BillboardSceneNode(){
		super();
		mNodeType = TYPE_BILLBOARD;
	}
	
	BillboardSceneNode(SceneNode node){
		Id = node.getId();
	}
	
	public void setColor(Color4i colorFront, Color4i colorBack){
		nativeSetColor(colorFront.r(), colorFront.g(), colorFront.b(), colorFront.a(),
				colorBack.r(), colorBack.g(), colorBack.b(), colorBack.a(), getId());
	}
	
	@Override
	public void setTouchable(boolean flag){
		nativeSetTouchable(flag, getId());
	}

	@Override
	public void setAmbientColor(Color4i color, int materialId) {
		nativeSetAmbientColor(color.r(), color.g(), color.b(), color.a(), materialId, getId());
	}

	@Override
	public void setDiffuseColor(Color4i color, int materialId) {
		nativeSetDiffuseColor(color.r(), color.g(), color.b(), color.a(), materialId, getId());
	}

	@Override
	public void setEmissiveColor(Color4i color, int materialId) {
		nativeSetEmissiveColor(color.r(), color.g(), color.b(), color.a(), materialId, getId());
	}

	@Override
	public void setSpecularColor(Color4i color, int materialId) {
		nativeSetSpecularColor(color.r(), color.g(), color.b(), color.a(), materialId, getId());
	}

	@Override
	public void setShininess(double para, int materialId) {
		nativeSetShininess(para, materialId, getId());
	}

	@Override
	public int setTexture(String path, int materialId) {
		return nativeSetTexture(path, materialId, getId());
	}
	
	@Override
	public int addTextureAnimator(String[] path, int timePerFrame, boolean loop) {
		return nativeAddTextureAnimator(path, timePerFrame, loop, getId());
	}
	
	private native void nativeSetColor(int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int id);
}
