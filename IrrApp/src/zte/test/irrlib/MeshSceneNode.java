package zte.test.irrlib;

public class MeshSceneNode extends SceneNode implements MaterialOwner{

	MeshSceneNode(){
		super();
		mNodeType = TYPE_MESH;
	}
	
	protected boolean mIsBBoxVisible = false;
	
	@Override
	public void setTouchable(boolean flag){
		nativeSetTouchable(flag, getID());
	}
	
	@Override
	public void setAmbientColor(Color4i color, int materialID) {
		nativeSetAmbientColor(color.r(), color.g(), color.b(), color.a(), materialID, getID());
	}

	@Override
	public void setDiffuseColor(Color4i color, int materialID) {
		nativeSetDiffuseColor(color.r(), color.g(), color.b(), color.a(), materialID, getID());
	}

	@Override
	public void setEmissiveColor(Color4i color, int materialID) {
		nativeSetEmissiveColor(color.r(), color.g(), color.b(), color.a(), materialID, getID());
	}

	@Override
	public void setSpecularColor(Color4i color, int materialID) {
		nativeSetSpecularColor(color.r(), color.g(), color.b(), color.a(), materialID, getID());
	}

	@Override
	public void setShininess(double para, int materialID) {
		nativeSetShininess(para, materialID, getID());
	}

	@Override
	public int setTexture(String path, int materialID) {
		return nativeSetTexture(path, materialID, getID());
	}
	
	@Override
	public int addTextureAnimator(String[] path, int timePerFrame, boolean loop) {
		return nativeAddTextureAnimator(path, timePerFrame, loop, getID());
	}
	
	public boolean isBBoxVisible(){
		return mIsBBoxVisible;
	}
	
	public void setBBoxVisibility(boolean flag){
		mIsBBoxVisible = flag;
		nativeSetBBoxVisibility(flag);
	}
}
