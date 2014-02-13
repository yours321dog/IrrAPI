package zte.irrlib.scene;

import java.util.ArrayList;
import java.util.Iterator;


public class BillboardGroup extends SceneNode{
	
	private ArrayList<BillboardSceneNode> mGroup;
	private Scene mScene;
	
	public final int REMOVE_FROM_SCENE = 0x01;
	public final int REMOVE_FROM_GROUP = 0x02;
	
	BillboardGroup(){
		super();
		mNodeType = TYPE_BILLBOARD_GROUP;
	}
	
	public void add(BillboardSceneNode node){
		node.setParent(this);
		node.setVisible(true);
		mGroup.add(node);
	}
	
	public boolean remove(BillboardSceneNode node, int mode){
		if (mode == REMOVE_FROM_SCENE){
			node.remove();
		}
		node.setParent(null);
		return mGroup.remove(node);
	}
	
	public void removeAll(int mode){
		
		Iterator<BillboardSceneNode> itr = mGroup.iterator();
		BillboardSceneNode tmp;
		while (itr.hasNext()){
			tmp = itr.next();
			tmp.setParent(null);
			if (mode == REMOVE_FROM_SCENE){
				tmp.remove();
			}
		}
		mGroup.removeAll(mGroup);
	}
	
	public void setViewCompressionValues(double near, double far){
		Iterator<BillboardSceneNode> itr = mGroup.iterator();
		CameraSceneNode camera = mScene.getActiveCamera();
		BillboardSceneNode tmp;
		double disSquare;
		
		while (itr.hasNext()){
			tmp = itr.next();
			disSquare = tmp.getPosition().distanceSquare(camera.getPosition());
			
			if (disSquare < near*near || disSquare > far*far){
				tmp.setVisible(false);
			}
			else tmp.setVisible(true);
		}
	}
}
