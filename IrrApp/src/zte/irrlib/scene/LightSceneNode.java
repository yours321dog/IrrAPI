 package zte.irrlib.scene;

import zte.irrlib.core.SLight;

public class LightSceneNode extends SceneNode {
	LightSceneNode(){
		super();
		mNodeType = TYPE_LIGHT;
	}
	
	public void UpdateLightData(){
		nativeSendLightData(LightData, getId());
	}
	
	public int getLightType(){
		return LightData.Type;
	}
	
	public SLight LightData;
	
	private native void nativeSendLightData(SLight data, int id);
}
