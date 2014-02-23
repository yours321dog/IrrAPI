package zte.irrlib.scene;

import zte.irrlib.core.Color4i;
import android.graphics.Bitmap;

public class MeshSceneNode extends SceneNode{

	MeshSceneNode(){
		super();
		mNodeType = TYPE_MESH;
	}
	
	public void setTouchable(boolean flag){
		nativeSetTouchable(flag, getId());
	}
	
	public void setSmoothShade(boolean flag, int materialId){
		nativeSetSmoothShade(flag, materialId, getId());
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
		return nativeSetTexture(mScene.getFullPath(path), materialId, getId());
	}
	
	public int setTexture(Bitmap bitmap, int materialId){
		return nativeSetBitmapTexture(bitmap.toString(), bitmap, materialId, getId());
	}
	
	public void setMediaTexture(int materialId){
		nativeSetMediaTexture(materialId, getId());
	}
	
	public int addTextureAnimator(String[] path, int timePerFrame, boolean loop) {
		return nativeAddTextureAnimator(path, timePerFrame, loop, getId());
	}
	
	public void setBBoxVisibility(boolean flag){
		nativeSetBBoxVisibility(flag, getId());
	}
	
	public int getMaterialCount(){
		return nativeGetMaterialCount(getId());
	}
	
	protected native int nativeSetTouchable(boolean flag, int Id);
	protected native int nativeSetBBoxVisibility(boolean flag, int Id);
	
	protected native int nativeSetSmoothShade(boolean flag, int materialId, int Id);
	protected native int nativeSetAmbientColor(int r, int g, int b, int a, int materialId, int Id);
	protected native int nativeSetDiffuseColor(int r, int g, int b, int a, int materialId, int Id);
	protected native int nativeSetEmissiveColor(int r, int g, int b, int a, int materialId, int Id);
	protected native int nativeSetSpecularColor(int r, int g, int b, int a, int materialId, int Id);
	protected native int nativeSetShininess(double shininess, int materialId, int Id);
	
	protected native int nativeSetTexture(String path, int materialId, int Id);
	protected native int nativeAddTextureAnimator(String[] path, int timePerFrame, boolean loop, int Id);
	private native int nativeSetBitmapTexture(String name, Bitmap bitmap, int materialId, int Id);
	private native int nativeSetMediaTexture(int materialId, int Id);
	private native int nativeGetMaterialCount(int Id);
}
