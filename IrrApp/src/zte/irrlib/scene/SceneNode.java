package zte.irrlib.scene;

import zte.irrapp.WLog;
import zte.irrlib.core.Color4i;
import zte.irrlib.core.Vector3d;

public class SceneNode {
	
	/*public static final SceneNode createNullNode(){
		SceneNode node = new SceneNode();
		node.Id = 0;
		node.mNodeType = TYPE_NULL;
		return node;
	}*/
	
	//public static final SceneNode NULL_SCENE_NODE;
	
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
	
	SceneNode() {
		Id = mScene.getNewId();
		
		//��һ���ǵ�ǰֵ���ڶ����Ǳ��ֵ
		mPosition = new Vector3d[2];
		mPosition[0] = new Vector3d(0, 0, 0);
		mPosition[1] = new Vector3d();
		
		mRotation = new Vector3d[2];
		mRotation[0] = new Vector3d(0, 0, 0);
		mRotation[1] = new Vector3d();
		
		mScale = new Vector3d[2];
		mScale[0] = new Vector3d(1, 1, 1);
		mScale[1] = new Vector3d();
		
		mark();
		
		mNodeType = TYPE_COMMON;
	}
	
	int getId() {return Id;}
	
	protected int Id;
	protected SceneNode parent = null;
	
	protected boolean mIsVisible = true;;
	protected Vector3d []mPosition;
	protected Vector3d []mRotation;
	protected Vector3d []mScale;
	
	protected int mNodeType;
	private static Scene mScene;
	
	public int getNodeType(){
		return mNodeType;
	}
	
	public static boolean setScene(Scene scene){
		if (scene == null) return false;
		mScene = scene;
		return true;
	}
	
	public void mark(){
		mPosition[1].copy(mPosition[0]);
		mRotation[1].copy(mRotation[0]);
		mScale[1].copy(mScale[0]);
	}
	
	public void setParent(SceneNode node){
		parent = node;
		nativeSetParent(mScene.getId(parent), getId());
	}
	
	public void setVisible(boolean isVisible){
		mIsVisible = isVisible;
		nativeSetVisible(isVisible, getId());
	}
	
	public void setRotation(Vector3d para, int mode){
		if (mode == ABSOLUTE_TRANSFORM){
			mRotation[0] = mRotation[1].plus(para);
		}
		else if (mode == RELATIVE_TRANSFORM){
			mRotation[0] = mRotation[0].plus(para);
		}
		else return;
		
		nativeSetRotation(mRotation[0].x, mRotation[0].y, mRotation[0].z, getId());
	}
	
	public void setTranslation(Vector3d para, int mode){
		if (mode == ABSOLUTE_TRANSFORM){
			mPosition[0] = mPosition[1].plus(para);
		}
		else if (mode == RELATIVE_TRANSFORM){
			mPosition[0] = mPosition[0].plus(para);
		}
		else return;
		
		nativeSetPosition(mPosition[0].x, mPosition[0].y, mPosition[0].z, getId());
	}
	public void setScale(Vector3d para, int mode){
		if (mode == ABSOLUTE_TRANSFORM){
			mScale[0] = mScale[1].plus(para);
		}
		else if (mode == RELATIVE_TRANSFORM){
			mScale[0] = mScale[0].plus(para);
		}
		else return;
		
		nativeSetScale(mScale[0].x, mScale[0].y, mScale[0].z, getId());
	}
	public void setPosition(Vector3d pos){
		mPosition[0] = pos;
		nativeSetPosition(mPosition[0].x, mPosition[0].y, mPosition[0].z, getId());
	}
	
	public Vector3d getPosition(){
		return mPosition[0];
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

	public void addRotationAnimator(Vector3d speed, int animatorId){
		nativeAddRotationAnimator(speed.x, speed.y, speed.z, getId());
	}
	
	public void addFlyCircleAnimator(Vector3d center, double radius, double speed, Vector3d axis){
		nativeAddFlyCircleAnimator(center.x, center.y, center.z, radius, speed, axis.x, axis.y, axis.z, getId());
	}
	
	public void addFlyStraightAnimator(Vector3d start, Vector3d end, double time){
		nativeAddFlyStraightAnimator(start.x, start.y, start.z, end.x, end.y, end.z, time, getId());
	}
	
	public void addDeleteAnimator(int ms){
		nativeAddDleeteAnimator(ms, getId());
	}
	
	//����ȥ�������ĺ���ʵ����������Զ࣬�ݲ���
	public void removeAllAnimator(){
		nativeRemoveAllAnimator(getId());
	}
	
	public void remove(){
		mScene.removeNode(this);
	}
	
	public interface MaterialOwner {
		
		void setAmbientColor(Color4i color, int materialId);
		void setDiffuseColor(Color4i color, int materialId);
		void setEmissiveColor(Color4i color, int materialId);
		void setSpecularColor(Color4i color, int materialId);
		void setShininess(double para, int materialId);
		
		//return 0 if succeed
		int setTexture(String path, int materialId);
		int addTextureAnimator(String[] path, int timePerFrame, boolean loop);
		
		void setTouchable(boolean flag);
	}

	private native void nativeSetParent(int parent, int Id);
	private native void nativeSetVisible(boolean isVisible, int Id);
	private native void nativeSetRotation(double x, double y, double z, int Id);
	private native void nativeSetScale(double x, double y, double z, int Id);
	private native void nativeSetPosition(double x, double y, double z, int Id);

	private native void nativeAddRotationAnimator(double x, double y, double z, int Id);
	private native void nativeAddFlyCircleAnimator(double cx, double cy, double cz,
			double radius, double speed, double ax, double ay, double az, int Id);
	private native void nativeAddFlyStraightAnimator(double sx, double sy, double sz,
			double dx, double dy, double dz, double time, int Id);
	private native void nativeAddDleeteAnimator(int ms, int Id);//added in 1.24

	private native void nativeRemoveAllAnimator(int Id);
	
	//only available with mesh node below.
	protected native void nativeSetTouchable(boolean flag, int Id);
	protected native void nativeSetBBoxVisibility(boolean flag);
	
	protected native void nativeSetAmbientColor(int r, int g, int b, int a, int materialId, int Id);
	protected native void nativeSetDiffuseColor(int r, int g, int b, int a, int materialId, int Id);
	protected native void nativeSetEmissiveColor(int r, int g, int b, int a, int materialId, int Id);
	protected native void nativeSetSpecularColor(int r, int g, int b, int a, int materialId, int Id);
	protected native void nativeSetShininess(double shininess, int materialId, int Id);
	
	protected native int nativeSetTexture(String path, int materialId, int Id);
	protected native int nativeAddTextureAnimator(String[] path, int timePerFrame, boolean loop, int Id);
}
