 package zte.irrlib.scene;

import zte.irrapp.WLog;
import zte.irrlib.core.SLight;

public class LightSceneNode extends SceneNode {
	LightSceneNode(){
		super();
		mNodeType = TYPE_LIGHT;
		LightData = new SLight();
	}
	
	public void upLoadLightData(){
		nativeSendLightData(LightData, getId());
	}
	
	public void downloadLightData(){
		nativeGetLightData(LightData, getId());
	}
	
	public int getLightType(){
		return LightData.Type;
	}
	
	public SLight LightData;
	
	private native int nativeSendLightData(SLight data, int id);
	private native int nativeGetLightData(SLight data, int id);
}
