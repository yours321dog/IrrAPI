package zte.irrlib.scene;


public class AnimateMeshSceneNode extends MeshSceneNode {
	
	AnimateMeshSceneNode(){
		super();
		mNodeType = TYPE_ANIMATE_MESH;
	}
	
	public int getStartFrame(){
		return nativeGetStartFrame(getId());
	}
	
	public int getEndFrame(){
		return nativeGetEndFrame(getId());
	}
	
	public int getFrameNumber(){
		return nativeGetFrameNumber(getId());
	}
	
	//return 0 means succeed
	public int setCurrentFrame(int frame){
		return nativeSetCurrentFrame(frame, getId());
	}
	
	public void setAnimationSpeed(double fps){
		nativeSetAnimationSpeed(fps, getId());
	}
	
	public void setLoopMode(boolean loop){
		nativeSetLoopMode(loop, getId());
	}
	
	private native int nativeGetStartFrame(int id);
	private native int nativeGetEndFrame(int id);
	private native int nativeGetFrameNumber(int id);
	private native int nativeSetCurrentFrame(int frame, int id);
	private native void nativeSetAnimationSpeed(double fps, int id);
	private native void nativeSetLoopMode(boolean loop, int id);
	
	
}