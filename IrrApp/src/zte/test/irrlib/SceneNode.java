package zte.test.irrlib;

public class SceneNode {
	
	public static final SceneNode createNullNode(){
		SceneNode node = new SceneNode(0);
		node.mNodeType = TYPE_NULL;
		return node;
	}
	
	public static final SceneNode NULL_SCENE_NODE = createNullNode();
	
	public static final int TYPE_NULL = 0x00;
	public static final int TYPE_COMMON = 0x01;
	public static final int TYPE_MESH = 0x02;
	public static final int TYPE_ANIMATE_MESH = 0x03;
	public static final int TYPE_LIGHT = 0x04;
	public static final int TYPE_BILLBOARD = 0x05;
	public static final int TYPE_BILLBOARD_GROUP = 0x06;
	public static final int TYPE_CAMERA = 0x07;
	public static final int TYPE_PARTICLE_SYSTEM = 0x08;
	
	public static final int ABSOLUTE_TRANSFORM = 0;
	public static final int RELATIVE_TRANSFORM = 1;
	
	public class TransformationInfo{
		public Vector3d Position;
		public Vector3d Rotation;
		public Vector3d Scale;
	}
	
	protected SceneNode(int id) {this.id = id;}
	
	SceneNode() {
		this.id = NodeFactory.getInstance().applyNewID();
		
		//第一个是当前值，第二个是标记值
		mPosition = new Vector3d[2];
		mPosition[1] = new Vector3d(0, 0, 0);
		mPosition[2] = new Vector3d();
		
		mRotation = new Vector3d[2];
		mRotation[1] = new Vector3d(0, 0, 0);
		mRotation[2] = new Vector3d();
		
		mScale = new Vector3d[2];
		mScale[1] = new Vector3d(1, 1, 1);
		mScale[2] = new Vector3d();
		
		mark();
		
		mNodeType = TYPE_COMMON;
		setTouchable(false);
	}
	
	int getID() {return id;}
	void setID(int id) {this.id = id;}//typically, we do not allow users to modify node IDs;
	
	protected int id;
	protected SceneNode parent = NULL_SCENE_NODE;
	
	protected boolean mIsVisible = true;;
	protected Vector3d []mPosition;
	protected Vector3d []mRotation;
	protected Vector3d []mScale;
	protected int mNodeType;
	protected boolean mIsTouchable;
	
	public void setTouchable(boolean flag){
		mIsTouchable = flag;
	}
	
	public int getNodeType(){
		return mNodeType;
	}
	
	public void mark(){
		mPosition[2].copy(mPosition[1]);
		mRotation[2].copy(mRotation[1]);
		mScale[2].copy(mScale[1]);
	}
	
	public void setParent(SceneNode node){
		if (node == null){
			parent = NULL_SCENE_NODE;
		}
		else parent = node;
		
		nativeSetParent(parent.getID(), getID());
	}
	
	public void setVisible(boolean isVisible){
		mIsVisible = isVisible;
		nativeSetVisible(isVisible, getID());
	}
	
	public void setRotation(Vector3d para, int mode){
		if (mode == ABSOLUTE_TRANSFORM){
			mRotation[1] = mRotation[2].plus(para);
		}
		else if (mode == RELATIVE_TRANSFORM){
			mRotation[1] = mRotation[1].plus(para);
		}
		else return;
		
		nativeSetRotation(mRotation[1].x, mRotation[1].y, mRotation[1].z, getID());
	}
	
	public void setTranslation(Vector3d para, int mode){
		if (mode == ABSOLUTE_TRANSFORM){
			mPosition[1] = mPosition[2].plus(para);
		}
		else if (mode == RELATIVE_TRANSFORM){
			mPosition[1] = mPosition[1].plus(para);
		}
		else return;
		
		nativeSetTranslation(mPosition[1].x, mPosition[1].y, mPosition[1].z, getID());
	}
	public void setScale(Vector3d para, int mode){
		if (mode == ABSOLUTE_TRANSFORM){
			mScale[1] = mScale[2].plus(para);
		}
		else if (mode == RELATIVE_TRANSFORM){
			mScale[1] = mScale[1].plus(para);
		}
		else return;
		
		nativeSetScale(mScale[1].x, mScale[1].y, mScale[1].z, getID());
	}
	public void setPosition(Vector3d pos){
		mPosition[1] = pos;
		nativeSetPosition(mPosition[1].x, mPosition[1].y, mPosition[1].z, getID());
	}
	
	public Vector3d getPosition(){
		return mPosition[1];
	}
	
	public TransformationInfo getTransformationInfo(){
		TransformationInfo info = new TransformationInfo();
		info.Position = new Vector3d(mPosition[1]);
		info.Rotation = new Vector3d(mRotation[1]);
		info.Scale = new Vector3d(mScale[1]);
		
		return info;
	}
	
	public void setTransformationInfo(TransformationInfo info){
		mPosition[1].copy(info.Position);
		mRotation[1].copy(info.Rotation);
		mScale[1].copy(info.Scale);
	}

	public void addRotationAnimator(Vector3d speed, int animatorID){
		nativeAddRotationAnimator(speed.x, speed.y, speed.z, getID());
	}
	
	public void addFlyCircleAnimator(Vector3d center, double radius, double speed, Vector3d axis){
		nativeAddFlyCircleAnimator(center.x, center.y, center.z, radius, speed, axis.x, axis.y, axis.z, getID());
	}
	
	public void addFlyStraightAnimator(Vector3d start, Vector3d end, double time){
		nativeAddFlyStraightAnimator(start.x, start.y, start.z, end.x, end.y, end.z, time, getID());
	}
	
	public void addDeleteAnimator(int ms){
		nativeAddDleeteAnimator(ms, getID());
	}
	
	//单个去除动画的函数实现所需代码略多，暂不管
	public void removeAllAnimator(){
		nativeRemoveAllAnimator(getID());
	}
	
	public void remove(){
		Device.getInstance().getNodeFactory().removeNode(this);
	}

	private native void nativeSetParent(int parent, int id);
	private native void nativeSetVisible(boolean isVisible, int id);
	private native void nativeSetRotation(double x, double y, double z, int id);
	private native void nativeSetTranslation(double x, double y, double z, int id);//name changed in 1.24
	private native void nativeSetScale(double x, double y, double z, int id);
	private native void nativeSetPosition(double x, double y, double z, int id);

	private native void nativeAddRotationAnimator(double x, double y, double z, int id);
	private native void nativeAddFlyCircleAnimator(double cx, double cy, double cz,
			double radius, double speed, double ax, double ay, double az, int id);
	private native void nativeAddFlyStraightAnimator(double sx, double sy, double sz,
			double dx, double dy, double dz, double time, int id);
	private native void nativeAddDleeteAnimator(int ms, int id);//added in 1.24
	
	//deprecated in 1.24
	//private native void nativeSetTexture(String path, int id);
	//private native void nativeSetTexture(String path, rect.left, rect.up, rect.right, rect.down, int id);
	//added in 1.24
	private native void nativeRemoveAllAnimator(int id);
}
