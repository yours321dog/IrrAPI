package zte.test.irrlib;

public class CameraSceneNode extends SceneNode{
	
	CameraSceneNode(){
		super();
		mNodeType = TYPE_CAMERA;
	}
	
	public CameraSceneNode(SceneNode node){
		id = node.getID();
	}
	
	public void setRendererClippingPlaneLimits(double nearClip, double farClip){
		nativeSetClipPlane(nearClip, farClip, getID());
	}
	
	protected Vector3d mLookAt;
	
	public void setLookAt(Vector3d vec){
		mLookAt = vec;
		nativeSetLookAt(mLookAt.x, mLookAt.y, mLookAt.z, getID());
	}
	
	private native void nativeSetLookAt(double x, double y, double z, int id);
	private native void nativeSetClipPlane(double nearClip, double farClip, int id);
}
