package zte.irrlib.scene;

import zte.irrlib.core.SLight;

public class LightSceneNode extends SceneNode {
	LightSceneNode(){
		super();
		mNodeType = TYPE_LIGHT;
	}
	
	public LightSceneNode(SceneNode node){
		this.Id = node.getId();
	}
	
	public SLight LightData;
	
	public void UpdateLightData(){
		nativeSendLightData(LightData, getId());
	}
	
	public int getLightType(){
		return LightData.Type;
	}
	
	private native void nativeSendLightData(SLight data, int id);
}
