package zte.irrlib.scene;

import zte.irrlib.core.Color4i;
import zte.irrlib.core.Vector2d;
import zte.irrlib.core.Vector3d;

class ParticleSystemSceneNode extends SceneNode {
	
	public static final int EMITTER_BOX = 0x01;
	
	public ParticleSystemSceneNode(){
		super();
		mNodeType = TYPE_PARTICLE_SYSTEM;
	}
	
	public void SetEmitter(int EmitterType, Param para){
		nativeSetEmitter(EmitterType, para, getId());
	}
	
	public class Param{
		public double BBox[];
		public Vector3d InitialDirection;
		public double MinEmitRate, MaxEmitRate;
		public Color4i DarkestColor, BrightestColor;
		public double MinLifeTime, MaxLifeTime;
		public double MaxAngleDegrees;
		public Vector2d MinSize, MaxSize;
	}
	
	private native void nativeSetEmitter(int type, Param para, int id);
}
