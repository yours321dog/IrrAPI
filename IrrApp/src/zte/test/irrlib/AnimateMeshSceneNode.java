package zte.test.irrlib;

public class AnimateMeshSceneNode extends MeshSceneNode {
	
	AnimateMeshSceneNode(){
		super();
		mNodeType = TYPE_ANIMATE_MESH;
	}
	
	public int getStartFrame(){
		return nativeGetStartFrame(getID());
	}
	
	public int getEndFrame(){
		return nativeGetEndFrame(getID());
	}
	
	public int getFrameNumber(){
		return nativeGetFrameNumber(getID());
	}
	
	//return 0 means succeed
	public int setCurrentFrame(int frame){
		return nativeSetCurrentFrame(frame, getID());
	}
	
	public void setAnimationSpeed(double fps){
		nativeSetAnimationSpeed(fps, getID());
	}
	
	public void setLoopMode(boolean loop){
		nativeSetLoopMode(loop, getID());
	}
	
	private native int nativeGetStartFrame(int id);
	private native int nativeGetEndFrame(int id);
	private native int nativeGetFrameNumber(int id);
	private native int nativeSetCurrentFrame(int frame, int id);
	private native void nativeSetAnimationSpeed(double fps, int id);
	private native void nativeSetLoopMode(boolean loop, int id);
	
	
}