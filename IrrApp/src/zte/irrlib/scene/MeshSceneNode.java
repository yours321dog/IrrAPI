package zte.irrlib.scene;

import zte.irrlib.core.Color4i;
import android.graphics.Bitmap;

public class MeshSceneNode extends SceneNode{

	MeshSceneNode(){
		super();
		mNodeType = TYPE_MESH;
	}
	
	protected boolean mIsBBoxVisible = false;
	
	public void setTouchable(boolean flag){
		nativeSetTouchable(flag, getId());
	}
	
	public void setAmbientColor(Color4i color, int materialId) {
		nativeSetAmbientColor(color.r(), color.g(), color.b(), color.a(), materialId, getId());
	}

	public void setDiffuseColor(Color4i color, int materialId) {
		nativeSetDiffuseColor(color.r(), color.g(), color.b(), color.a(), materialId, getId());
	}

	public void setEmissiveColor(Color4i color, int materialId) {
		nativeSetEmissiveColor(color.r(), color.g(), color.b(), color.a(), materialId, getId());
	}

	public void setSpecularColor(Color4i color, int materialId) {
		nativeSetSpecularColor(color.r(), color.g(), color.b(), color.a(), materialId, getId());
	}

	public void setShininess(double para, int materialId) {
		nativeSetShininess(para, materialId, getId());
	}

	public int setTexture(String path, int materialId) {
		return nativeSetTexture(path, materialId, getId());
	}
	
	public int setTexture(Bitmap bitmap, int materialId){
		return nativeSetBitmapTexture(bitmap.toString(), bitmap, materialId, getId());
	}
	
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
	
	protected native void nativeSetTouchable(boolean flag, int Id);
	protected native void nativeSetBBoxVisibility(boolean flag);
	
	protected native void nativeSetAmbientColor(int r, int g, int b, int a, int materialId, int Id);
	protected native void nativeSetDiffuseColor(int r, int g, int b, int a, int materialId, int Id);
	protected native void nativeSetEmissiveColor(int r, int g, int b, int a, int materialId, int Id);
	protected native void nativeSetSpecularColor(int r, int g, int b, int a, int materialId, int Id);
	protected native void nativeSetShininess(double shininess, int materialId, int Id);
	
	protected native int nativeSetTexture(String path, int materialId, int Id);
	protected native int nativeAddTextureAnimator(String[] path, int timePerFrame, boolean loop, int Id);
	private native int nativeSetBitmapTexture(String name, Bitmap bitmap, int materialId, int id);
}
