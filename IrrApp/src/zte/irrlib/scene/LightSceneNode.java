 package zte.irrlib.scene;

import zte.irrlib.core.SLight;

public class LightSceneNode extends SceneNode {
	LightSceneNode(){
		super();
		mNodeType = TYPE_LIGHT;
		LightData = new SLight();
	}
	
	public void UpdateLightData(){
		nativeSendLightData(LightData, getId());
	}
	
	public int getLightType(){
		return LightData.Type;
	}
	
	public SLight LightData;
	
	private native int nativeSendLightData(SLight data, int id);
}
