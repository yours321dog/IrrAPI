package zte.irrlib.scene;

import zte.irrlib.core.Vector2i;
import zte.irrlib.core.Vector3d;

public class CameraSceneNode extends SceneNode{
	
	public void setRendererClippingPlaneLimits(double nearClip, double farClip){
		nativeSetClipPlain(nearClip, farClip, getId());
	}
	
	public void setLookAt(Vector3d vec){
		nativeSetLookAt(vec.x, vec.y, vec.z, getId());
	}
	
	public void setAspectRatio(double ratio){
		nativeSetAspectRatio(ratio, getId());
	}
	
	public void setFovy(double fovy){
		nativeSetFovy(fovy, getId());
	}
	
	public void javaLoadDataAndInit(Vector3d pos, Vector3d lookAt, SceneNode parent){
		super.javaLoadDataAndInit(pos, parent);
		Vector2i size = mScene.getRenderSize();
		setAspectRatio((float)size.x/size.y);
	}
	
	CameraSceneNode(){
		super();
		mNodeType = TYPE_CAMERA;
	}
	
	private native void nativeSetLookAt(double x, double y, double z, int id);
	private native void nativeSetClipPlain(double nearClip, double farClip, int id);
	private native void nativeSetAspectRatio(double ratio, int id);
	private native void nativeSetFovy(double fovy, int id);
}
