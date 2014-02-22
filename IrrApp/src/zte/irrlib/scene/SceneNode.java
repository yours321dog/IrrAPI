package zte.irrlib.scene;

import zte.irrlib.core.Vector3d;

public class SceneNode {
	
	public static final int TYPE_NULL = 0x00000000;
	public static final int TYPE_COMMON = 0x00000001;
	public static final int TYPE_MESH = 0x00000102;
	public static final int TYPE_ANIMATE_MESH = 0x00000103;
	public static final int TYPE_LIGHT = 0x00000004;
	public static final int TYPE_BILLBOARD = 0x00001005;
	public static final int TYPE_BILLBOARD_GROUP = 0x00000006;
	public static final int TYPE_CAMERA = 0x00000007;
	public static final int TYPE_PARTICLE_SYSTEM = 0x00000008;
	
	public static final int TRANS_ABSOLUTE = 0;
	public static final int TRANS_RELATIVE = 1;
	
	public static final int FLAG_MATERIAL_OWNER = 0x00001000;
	
	public static final void setScene(Scene scene){
		mScene = scene;
	}
	
	public final int getNodeType(){
		return mNodeType;
	}

	public void mark(){
		mPosition[1].copy(mPosition[0]);
		mRotation[1].copy(mRotation[0]);
		mScale[1].copy(mScale[0]);
	}
	
	public void setParent(SceneNode node){
		mParent = node;
		nativeSetParent(mScene.getId(mParent), getId());
	}
	
	public void setVisible(boolean isVisible){
		mIsVisible = isVisible;
		nativeSetVisible(isVisible, getId());
	}

	public void setPosition(Vector3d para, int mode){
		if (mode == TRANS_ABSOLUTE){
			mPosition[0].copy(para);
		}
		else if (mode == TRANS_RELATIVE){
			mPosition[0] = mPosition[1].plus(para);
		}
		else return;
		
		nativeSetPosition(mPosition[0].x, mPosition[0].y, mPosition[0].z, getId());
	}

	public void setRotation(Vector3d para, int mode){
		if (mode == TRANS_ABSOLUTE){
			mRotation[0].copy(para);
		}
		else if (mode == TRANS_RELATIVE){
			mRotation[0] = mRotation[1].plus(para);
		}
		else return;
		
		nativeSetRotation(mRotation[0].x, mRotation[0].y, mRotation[0].z, getId());
	}

	public void setScale(Vector3d para, int mode){
		if (mode == TRANS_ABSOLUTE){
			mScale[0].copy(para);
		}
		else if (mode == TRANS_RELATIVE){
			mScale[0] = mScale[1].multi(para);
		}
		else return;
		
		nativeSetScale(mScale[0].x, mScale[0].y, mScale[0].z, getId());
	}

	public Vector3d getPosition(){
		return new Vector3d(mPosition[0]);
	}
	
	public Vector3d getRotation(){
		return new Vector3d(mRotation[0]); 
	}
	
	public Vector3d getScale(){
		return new Vector3d(mScale[0]); 
	}
	
	public TransformationInfo getTransformationInfo(){
		TransformationInfo info = new TransformationInfo();
		info.Position = new Vector3d(mPosition[0]);
		info.Rotation = new Vector3d(mRotation[0]);
		info.Scale = new Vector3d(mScale[0]);
		
		return info;
	}
	
	public void setTransformationInfo(TransformationInfo info){
		mPosition[0].copy(info.Position);
		mRotation[0].copy(info.Rotation);
		mScale[0].copy(info.Scale);
	}

	public void addFlyStraightAnimator(Vector3d start, Vector3d end, 
			double time, boolean loop, boolean pingpong){
		nativeAddFlyStraightAnimator(start.x, start.y, start.z, 
				end.x, end.y, end.z, time, loop, pingpong, getId());
	}

	public void addFlyCircleAnimator(Vector3d center, double radius,
			double speed, Vector3d axis, double startPoint, double radiusEllipsoid){
		nativeAddFlyCircleAnimator(center.x, center.y, center.z, radius, speed, 
				axis.x, axis.y, axis.z, startPoint, startPoint, getId());
	}

	public void addRotationAnimator(Vector3d speed, int animatorId){
		nativeAddRotationAnimator(speed.x, speed.y, speed.z, getId());
	}
	
	public void addDeleteAnimator(int ms){
		nativeAddDeleteAnimator(ms, getId());
	}
	
	//单个去除动画的函数实现所需代码略多，暂不管
	public void removeAllAnimator(){
		nativeRemoveAllAnimator(getId());
	}
	
	public void remove(){
		mScene.removeNode(this);
	}	
	
	void javaLoadDataAndInit(Vector3d pos, SceneNode parent){
		mPosition[0] = pos;
		mParent = parent;
		mark();
		mScene.registerNode(this);
	}

	SceneNode() {
		this.Id = mScene.getNewId();
		
		//第一个是当前值，第二个是标记值
		mPosition = new Vector3d[2];
		mPosition[0] = new Vector3d(0, 0, 0);
		mPosition[1] = new Vector3d();
		
		mRotation = new Vector3d[2];
		mRotation[0] = new Vector3d(0, 0, 0);
		mRotation[1] = new Vector3d();
		
		mScale = new Vector3d[2];
		mScale[0] = new Vector3d(1, 1, 1);
		mScale[1] = new Vector3d();
		
		mNodeType = TYPE_COMMON;
	}
	
	int getId() {return Id;}
	
	protected static Scene mScene;
	
	protected final int Id;
	
	protected SceneNode mParent = null;
	protected boolean mIsVisible = true;
	protected Vector3d []mPosition;
	protected Vector3d []mRotation;
	protected Vector3d []mScale;
	
	protected int mNodeType;
	
	//! transform native method.
	private native int nativeSetParent(int parent, int Id);
	private native int nativeSetVisible(boolean isVisible, int Id);
	private native int nativeSetRotation(double x, double y, double z, int Id);
	private native int nativeSetScale(double x, double y, double z, int Id);
	private native int nativeSetPosition(double x, double y, double z, int Id);

	//! animator native method.
	private native int nativeAddRotationAnimator(
			double x, double y, double z, int Id);
			
	private native int nativeAddFlyCircleAnimator(
			double cx, double cy, double cz, double radius, double speed, 
			double ax, double ay, double az, double startPosition, 
			double radiusEllipsoid, int Id);
			
	private native int nativeAddFlyStraightAnimator(double sx, double sy, double sz,
			double dx, double dy, double dz, double time, 
			boolean loop, boolean pingpong, int Id);
			
	private native int nativeAddDeleteAnimator(int ms, int Id);
	private native int nativeRemoveAllAnimator(int Id);

	public class TransformationInfo{
		public Vector3d Position;
		public Vector3d Rotation;
		public Vector3d Scale;
	}
}
