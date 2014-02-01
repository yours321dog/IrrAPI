package zte.test.irrlib;

public class LightSceneNode extends SceneNode {
	LightSceneNode(){
		super();
		mNodeType = TYPE_LIGHT;
	}
	
	public LightSceneNode(SceneNode node){
		this.id = node.getID();
	}
	
	public SLight LightData;
	
	public void UpdateLightData(){
		nativeSendLightData(LightData, getID());
	}
	
	public int getLightType(){
		return LightData.Type;
	}
	
	private native void nativeSendLightData(SLight data, int id);
}
