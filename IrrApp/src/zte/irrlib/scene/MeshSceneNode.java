package zte.irrlib.scene;

import zte.irrlib.core.Color4i;

public class MeshSceneNode extends SceneNode implements SceneNode.MaterialOwner{

	MeshSceneNode(){
		super();
		mNodeType = TYPE_MESH;
	}
	
	protected boolean mIsBBoxVisible = false;
	
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
	
	public boolean isBBoxVisible(){
		return mIsBBoxVisible;
	}
	
	public void setBBoxVisibility(boolean flag){
		mIsBBoxVisible = flag;
		nativeSetBBoxVisibility(flag);
	}
}
