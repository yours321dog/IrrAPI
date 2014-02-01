package zte.test.irrlib;

class ParticleSystemSceneNode extends SceneNode {
	
	public static final int EMITTER_BOX = 0x01;
	
	public ParticleSystemSceneNode(){
		super();
		mNodeType = TYPE_PARTICLE_SYSTEM;
	}
	
	public void SetEmitter(int EmitterType, Param para){
		nativeSetEmitter(EmitterType, para, getID());
	}
	
	public class Param{
		double BBox[];
		Vector3d InitialDirection;
		double MinEmitRate, MaxEmitRate;
		Color4i DarkestColor, BrightestColor;
		double MinLifeTime, MaxLifeTime;
		double MaxAngleDegrees;
		Vector2d MinSize, MaxSize;
	}
	
	private native void nativeSetEmitter(int type, Param para, int id);
}
