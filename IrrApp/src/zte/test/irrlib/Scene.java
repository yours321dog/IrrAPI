package zte.test.irrlib;

import java.util.ArrayList;

public class Scene {
	
	public CameraSceneNode ActiveCamera;
	
	private Scene(){
		
	}
	
	public static Scene getInstance(){
		if (mUniInstance == null){
			mUniInstance = new Scene();
		}
		return mUniInstance;
	}
	
	private static Scene mUniInstance;
	private Device mDevice;
	private ArrayList<SceneNode> mNodeList;
	
	public void setIrrDevice(Device dev){
		mDevice = dev;
	}
	
	public void setClearColor(Color4i color){
		nativeSetClearColor(color.r(), color.g(), color.b(), color.a());
	}
	
	public void drawImage(String path, Vector2i leftUp, int w, int h, int transparent){
		if (transparent < 0) transparent = 0;
		else if (transparent > 255) transparent = 255;
		
		nativeDrawImage(mDevice.getPathPrefix() + path, leftUp.x, leftUp.y, w, h, transparent);
	}
	
	public void drawRectangle(Vector2i leftUp, int w, int h, Color4i color){
		nativeDrawRectangle(leftUp.x, leftUp.y, w, h, color.r(), color.g(), color.b(), color.a());
	}
	
	public void drawText(String text, Vector2i leftUp, float size, Color4i color){
		nativeDrawText(text, leftUp.x, leftUp.y, size, color.r(), color.g(), color.b(), color.a());
	}
	
	public void drawAllNodes(){nativeSmgrDrawAll();}
	
	public void setAmbientLight(Color4i color){
		nativeSetAmbientLight(color.r(), color.g(), color.b(), color.a());
	}
	
	public void setActiveCamera(CameraSceneNode camera){
		if (camera != null){
			ActiveCamera = camera;
			nativeSetActiveCamera(camera.getID());
		}
	}
	
	public SceneNode getTouchedSceneNode(int x, int y, SceneNode root){
		int nodeID = nativeGetTouchedSceneNode(x, y, NodeFactory.getNodeID(root));
		return queryByID(nodeID);
	}
	
	void registerNode(SceneNode node){
		if (node != null && node != SceneNode.NULL_SCENE_NODE){
			mNodeList.add(node);
		}
	}
	
	boolean unregisterNode(SceneNode node){
		if (node != null){
			for (SceneNode itr:mNodeList){
				if (itr.getID() == node.getID()){
					mNodeList.remove(itr);
					return true;
				}
			}
		}
		return false;
	}
	
	SceneNode queryByID(int id){
		if (id > 0){
			for (SceneNode node:mNodeList){
				if (node.getID() == id){
					return node;
				}
			}
		}
		return null;
	}
	
	private native void nativeSetClearColor(int r, int g, int b, int a);
	private native void nativeDrawImage(String path, int left, int up, int width, int height, int transparent);
	private native void nativeDrawRectangle(int left, int up, int width, int height, int r, int g, int b,int transparent);
	private native void nativeDrawText(String text, int left, int up, float size, int r, int g, int b, int a);
	
    private native void nativeSmgrDrawAll();//draw nodes method is moved here, in 1.24
    private native void nativeSetAmbientLight(int r, int g, int b, int a);//add in 1.24
    private native void nativeSetActiveCamera(int id);//add in 1.24
    private native int nativeGetTouchedSceneNode(int x, int y, int root);
	
}
