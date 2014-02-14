package zte.irrlib.scene;

import java.util.ArrayList;

import zte.irrapp.WLog;
import zte.irrlib.Engine;
import zte.irrlib.core.Color3i;
import zte.irrlib.core.Color4i;
import zte.irrlib.core.Vector2d;
import zte.irrlib.core.Vector2i;
import zte.irrlib.core.Vector3d;

public class Scene {
		
	public void setClearColor(Color4i color){
		nativeSetClearColor(color.r(), color.g(), color.b(), color.a());
	}
	
	public void setAmbientLight(Color4i color){
		nativeSetAmbientLight(color.r(), color.g(), color.b(), color.a());
	}
	
	public void setActiveCamera(CameraSceneNode camera){
		if (camera != null){
			mActiveCamera = camera;
			nativeSetActiveCamera(camera.getId());
		}
	}
	
	public CameraSceneNode getActiveCamera(){
		return mActiveCamera;
	}
		
	public SceneNode getTouchedSceneNode(int x, int y, SceneNode root){
		int nodeId = nativeGetTouchedSceneNode(x, y, getId(root));
		return queryById(nodeId);
	}
	
	public SceneNode queryById(int id){
		if (id > 0){
			for (SceneNode node:mNodeList){
				if (node.getId() == id){
					return node;
				}
			}
		}
		return null;
	}
	
	public int getId(SceneNode node){
		if (node == null){
			return 0;
		}
		else return node.getId();
	}
	
	int getNewId(){
		return ++_NewId;
	}
	
	public void drawImage(String path, Vector2i leftUp, int w, int h, int transparent){
		if (transparent < 0) transparent = 0;
		else if (transparent > 255) transparent = 255;	
		nativeDrawImage(mEngine.getResourceDir() + path, leftUp.x, leftUp.y, w, h, transparent);
	}
	
	public void drawRectangle(Vector2i leftUp, int w, int h, Color4i color){
		nativeDrawRectangle(leftUp.x, leftUp.y, w, h, color.r(), color.g(), color.b(), color.a());
	}
	
	public void drawText(String text, Vector2i leftUp, float size, Color4i color){
		nativeDrawText(text, leftUp.x, leftUp.y, size, color.r(), color.g(), color.b(), color.a());
	}
	
	public void drawAllNodes(){
		nativeSmgrDrawAll();
	}
	
	public SceneNode addEmptySceneNode(Vector3d pos, SceneNode parent){
		SceneNode node = new SceneNode();
		if (nativeAddEmptySceneNode(pos.x, pos.y, pos.z, getId(node), getId(parent)) != 0)
			return null;
		
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public MeshSceneNode addCubeSceneNode(Vector3d pos, float size, SceneNode parent){
		MeshSceneNode node = new MeshSceneNode();
		if (nativeAddCubeSceneNode(pos.x, pos.y, pos.z, size, getId(node), getId(parent)) != 0)
			return null;
		
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public MeshSceneNode addMeshSceneNode(String path, Vector3d pos, SceneNode parent){
		MeshSceneNode node = new MeshSceneNode();
		if (nativeAddMeshSceneNode(mEngine.getResourceDir() + path, pos.x, pos.y, pos.z, getId(node), getId(parent)) != 0)
			return null;
		
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public SceneNode addTextNode(String text, Vector3d pos, double size, SceneNode parent){
		SceneNode node = new SceneNode();
		if (nativeAddTextNode(text, pos.x, pos.y, pos.z, size, getId(node), getId(parent)) != 0)
			return null;
		
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public CameraSceneNode addCameraSceneNode(Vector3d pos, Vector3d lookAt, boolean isActive, SceneNode parent){
		CameraSceneNode node = new CameraSceneNode();
		if (nativeAddCameraSceneNode(pos.x, pos.y, pos.z, lookAt.x, lookAt.y, lookAt.z, isActive, getId(node), getId(parent)) != 0)
			return null;
		
		node.setLookAt(lookAt);
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public BillboardSceneNode addBillboardSceneNode(Vector3d pos, Vector2d size, SceneNode parent){
		BillboardSceneNode node = new BillboardSceneNode();
		if (nativeAddBillboardSceneNode(pos.x, pos.y, pos.z, size.x, size.y, getId(node), getId(parent)) != 0){
			return null;
		}
		
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public LightSceneNode addLightSceneNode(Vector3d pos, double radius, Color3i color, SceneNode parent){
		LightSceneNode node = new LightSceneNode();
		if (nativeAddLightSceneNode(pos.x, pos.y, pos.z, radius,
				color.r(), color.g(), color.b(), 
				getId(node), getId(parent)) != 0){
			return null;
		}
		
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public BillboardGroup addBillboardGroup(Vector3d pos, SceneNode parent){
		BillboardGroup node = new BillboardGroup();
		if (nativeAddEmptySceneNode(pos.x, pos.y, pos.z, getId(node), getId(parent)) != 0){
			return null;
		}
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public AnimateMeshSceneNode addAnimateMeshSceneNode(String path, Vector3d pos, SceneNode parent){
		AnimateMeshSceneNode node = new AnimateMeshSceneNode();
		if (nativeAddAnimateMeshSceneNode(path, pos.x, pos.y, pos.z, getId(node), getId(parent)) != 0){
			return null;
		}
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public ParticleSystemSceneNode addParticleSystemSceneNode(Vector3d pos, boolean withDefaultEmitter, SceneNode parent){
		ParticleSystemSceneNode node = new ParticleSystemSceneNode();
		if (nativeAddParticleSystemSceneNode(pos.x, pos.y, pos.z, withDefaultEmitter, getId(node), getId(parent)) != 0){
			return null;
		}
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}

	public void removeNode(SceneNode node){
		unregisterNode(node);
		nativeRemoveNode(node.getId());
	}
	
	public void init(){
		SceneNode.setScene(this);
		addCameraSceneNode(
				new Vector3d(0, 0, 0), 
				new Vector3d(0, 0, 100), true, null);
	}
	
	public void onResize(int width, int height){
		mWidth = width;
		mHeight = height;
	}
	
	public Vector2i getRenderSize(){
		return new Vector2i(mWidth, mHeight);
	}
	
	public void clearAllNodes(){
		mNodeList.clear();
		nativeClear();
		_NewId = 0;
	}
	
	public void javaClear(){
		mNodeList.clear();
		_NewId = 0;
	}
	
	//this method will *NOT* automatically register node in native engine
	//thus, it should not be used alone
	void registerNode(SceneNode node){
		if (node != null){
			mNodeList.add(node);
		}
	}
	
	//this method will *NOT* automatically unregister node in native engine
	//thus, it should not be used alone
	boolean unregisterNode(SceneNode node){
		if (node != null){
			for (SceneNode itr:mNodeList){
				if (getId(itr) == getId(node)){
					mNodeList.remove(itr);
					return true;
				}
			}
		}
		return false;
	}
		
	public static Scene getInstance(Engine engine){
		if (_UniInstance == null){
			_UniInstance = new Scene(engine);
		}
		else _UniInstance.mEngine = engine;
		
		return _UniInstance;
	}
	
	public static Scene getInstance(){
		if (_UniInstance == null || _UniInstance.mEngine == null)
			return null;
		
		return _UniInstance;
	}
	
	private static Scene _UniInstance;
	private static int _NewId;
	
	private Engine mEngine;
	private CameraSceneNode mActiveCamera;
	private SceneNode mRootNode;
	private ArrayList<SceneNode> mNodeList;
	private int mWidth, mHeight;
	
	private Scene(Engine engine){
		mEngine = engine;
		mNodeList = new ArrayList<SceneNode>();
	}
	
	private native void nativeSetClearColor(int r, int g, int b, int a);
    private native void nativeSetAmbientLight(int r, int g, int b, int a);
    private native void nativeSetActiveCamera(int id);    

    private native int nativeGetTouchedSceneNode(int x, int y, int root);
    
    //native draw API
	private native void nativeDrawImage(
			String path, int left, int up, 
			int width, int height, int transparent);
	
	private native void nativeDrawRectangle(
			int left, int up, int width, int height, 
			int r, int g, int b,int a);
	
	private native void nativeDrawText(
			String text, int left, int up,
			float size, int r, int g, int b, int a);
	
    private native void nativeSmgrDrawAll();

    
    //native add node API
	private native int nativeAddEmptySceneNode(
			double x, double y, double z, int id, int parent);
	
	private native int nativeAddCubeSceneNode(
			double x, double y, double z, 
			double size, int id, int parent);
	
	private native int nativeAddMeshSceneNode(
			String path, double x, double y, double z, int id, int parent);
	
	private native int nativeAddTextNode(
			String text, double x, double y, double z, 
			double size, int id, int parent);

	private native int nativeAddCameraSceneNode(
			double px, double py, double pz, 
			double lx, double ly, double lz, 
			boolean isActive, int id, int parent);
	
	private native int nativeAddBillboardSceneNode(
			double px, double py, double pz, 
			double sx, double sy, int id, int parent);
	
	private native int nativeAddLightSceneNode(
			double px, double py, double pz, double radius,
			int r, int g, int b, int id, int parent);
	
	private native int nativeAddAnimateMeshSceneNode(
			String path, double x, double y, double z, int id, int parent);
	
	private native int nativeAddParticleSystemSceneNode(
			double x, double y, double z, 
			boolean withDefaultEmitter, int id, int parent);
	
	//native remove node
	private native void nativeRemoveNode(int id);
	private native void nativeClear();
}
