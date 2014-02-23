package zte.irrlib.scene;

import java.util.ArrayList;

import zte.irrlib.Engine;
import zte.irrlib.Utils;
import zte.irrlib.core.Color3i;
import zte.irrlib.core.Color4i;
import zte.irrlib.core.Rect4i;
import zte.irrlib.core.Vector2d;
import zte.irrlib.core.Vector2i;
import zte.irrlib.core.Vector3d;

public class Scene {
	
	public static final String TAG = "Scene";
	
	public void enableLighting(boolean flag){
		mEnableLighting = flag;
	}

	public void setDefaultFontPath(String path){
		nativeSetFontPath(getFullPath(path));
	}
	
	public void setResourceDir(String path){
		mResourceDir = path;
	}

	public void setActiveCamera(CameraSceneNode camera){
		mActiveCamera = camera;
		nativeSetActiveCamera(getId(camera));
	}

	public void setClearColor(Color4i color){
		nativeSetClearColor(color.r(), color.g(), color.b(), color.a());
	}
	
	public void setAmbientLight(Color4i color){
		nativeSetAmbientLight(color.r(), color.g(), color.b(), color.a());
	}
	
	public String getResourceDir(){
		return mResourceDir;
	}

	public CameraSceneNode getActiveCamera(){
		return mActiveCamera;
	}
		
	public SceneNode getTouchedSceneNode(int x, int y, SceneNode root){
		return queryById(nativeGetTouchedSceneNode(x, y, getId(root)));
	}
	
	public Vector2i getRenderSize(){
		return new Vector2i(mWidth, mHeight);
	}
	
	public SceneNode queryById(int id){
		if (id <= 0) return null;
		for (SceneNode node:mNodeList){
			if (getId(node) == id){
				return node;
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
	
	//to be extend, can be transparent one, can be only part of it.
	public void drawImage(String path, Vector2i leftUp, Vector2i size){
		nativeDrawImage(getFullPath(path), leftUp.x, leftUp.y, size.x, size.y);
	}
	
	//to be extend, can be multi-color one
	public void drawRectangle(Vector2i leftUp, Vector2i size, Color4i color){
		nativeDrawRectangle(leftUp.x, leftUp.y, size.x, size.y, color.r(), color.g(), color.b(), color.a());
	}
	
	//It is necessary to supply font choice?
	public void drawText(String text, Vector2i leftUp, Color4i color){
		nativeDrawText(text, leftUp.x, leftUp.y, color.r(), color.g(), color.b(), color.a());
	}
	
	public void drawAllNodes(){
		nativeSmgrDrawAll();
	}
	
	public SceneNode addEmptySceneNode(Vector3d pos, SceneNode parent){
		SceneNode node = new SceneNode();
		if (nativeAddEmptySceneNode(pos.x, pos.y, pos.z, 
				getId(node), getId(parent), mEnableLighting) != 0)
			return null;
		
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public MeshSceneNode addCubeSceneNode(Vector3d pos, float size, SceneNode parent){
		MeshSceneNode node = new MeshSceneNode();
		if (nativeAddCubeSceneNode(pos.x, pos.y, pos.z, 
				size, getId(node), getId(parent), mEnableLighting) != 0)
			return null;
		
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public MeshSceneNode addMeshSceneNode(String path, Vector3d pos, SceneNode parent){
		MeshSceneNode node = new MeshSceneNode();
		if (nativeAddMeshSceneNode(getFullPath(path), pos.x, pos.y, pos.z,
				getId(node), getId(parent), mEnableLighting) != 0)
			return null;
		
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public SceneNode addTextNode(String text, Vector3d pos, double size, SceneNode parent){
		SceneNode node = new SceneNode();
		if (nativeAddTextNode(text, pos.x, pos.y, pos.z, size, 
				getId(node), getId(parent), mEnableLighting) != 0)
			return null;
		
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public CameraSceneNode addCameraSceneNode(Vector3d pos, Vector3d lookAt, boolean isActive, SceneNode parent){
		CameraSceneNode node = new CameraSceneNode();
		if (nativeAddCameraSceneNode(pos.x, pos.y, pos.z, 
				lookAt.x, lookAt.y, lookAt.z, isActive, 
				getId(node), getId(parent), mEnableLighting) != 0)
			return null;
		
		node.javaLoadDataAndInit(pos, lookAt, parent);
		return node;
	}
	
	public BillboardSceneNode addBillboardSceneNode(Vector3d pos, Vector2d size, SceneNode parent){
		BillboardSceneNode node = new BillboardSceneNode();
		if (nativeAddBillboardSceneNode(pos.x, pos.y, pos.z, 
				size.x, size.y, getId(node), getId(parent), mEnableLighting) != 0){
			return null;
		}
		
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public LightSceneNode addLightSceneNode(Vector3d pos, double radius, Color3i color, SceneNode parent){
		LightSceneNode node = new LightSceneNode();
		if (nativeAddLightSceneNode(pos.x, pos.y, pos.z, radius,
				color.r(), color.g(), color.b(), 
				getId(node), getId(parent), mEnableLighting) != 0){
			return null;
		}
		
		node.javaLoadDataAndInit(pos, parent);
		node.downloadLightData();
		return node;
	}
	
	public BillboardGroup addBillboardGroup(Vector3d pos, SceneNode parent){
		BillboardGroup node = new BillboardGroup();
		if (nativeAddEmptySceneNode(pos.x, pos.y, pos.z, 
				getId(node), getId(parent), mEnableLighting) != 0){
			return null;
		}
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public AnimateMeshSceneNode addAnimateMeshSceneNode(String path, Vector3d pos, SceneNode parent){
		AnimateMeshSceneNode node = new AnimateMeshSceneNode();
		if (nativeAddAnimateMeshSceneNode(getFullPath(path), pos.x, pos.y, pos.z,
				getId(node), getId(parent), mEnableLighting) != 0){
			return null;
		}
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public ParticleSystemSceneNode addParticleSystemSceneNode(Vector3d pos, boolean withDefaultEmitter, SceneNode parent){
		ParticleSystemSceneNode node = new ParticleSystemSceneNode();
		if (nativeAddParticleSystemSceneNode(pos.x, pos.y, pos.z, 
				withDefaultEmitter, getId(node), getId(parent), mEnableLighting) != 0){
			return null;
		}
		node.javaLoadDataAndInit(pos, parent);
		return node;
	}
	
	public TexMediaPlayer getMediaPlayer(){
		if (mMediaPlayer == null){
			mMediaPlayer = new TexMediaPlayer(this, nativeGetMediaTextureId());
		}
		return mMediaPlayer;
	}

	public void removeNode(SceneNode node){
		unregisterNode(node);
		nativeRemoveNode(node.getId());
	}
	
	public void clearAllNodes(){
		mNodeList.clear();
		nativeClear();
		_NewId = 0;
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
	
	public void javaClear(){
		mNodeList.clear();
		if (mMediaPlayer != null){
			mMediaPlayer.release();
			mMediaPlayer = null;
		}
		_NewId = 0;
	}
	
	//this method will *NOT* automatically register node in native engine
	//thus, it should not be used alone
	void registerNode(SceneNode node){
		mNodeList.add(node);
	}
	
	//this method will *NOT* automatically unregister node in native engine
	//thus, it should not be used alone
	boolean unregisterNode(SceneNode node){
		for (SceneNode itr:mNodeList){
			if (getId(itr) == getId(node)){
				mNodeList.remove(itr);
				return true;
			}
		}
		return false;
	}
	
	int getNewId(){
		return ++_NewId;
	}
		
	public static Scene getInstance(Engine engine){
		if (_UniInstance == null){
			_UniInstance = new Scene(engine);
		}else {
			_UniInstance.mEngine = engine;
		}
		return _UniInstance;
	}
	
	public static Scene getInstance(){
		if (_UniInstance == null || _UniInstance.mEngine == null)
			return null;
		
		return _UniInstance;
	}
	
	protected String getFullPath(String path){
		if (Utils.isAbsolutePath(path)){
			return path; 
		} else {
			return getResourceDir() + path;
		}
	}
	
	private static Scene _UniInstance;
	private static int _NewId;
	
	private Engine mEngine;
	private CameraSceneNode mActiveCamera;
	private ArrayList<SceneNode> mNodeList;
	private int mWidth, mHeight;
	private boolean mEnableLighting = true;
	private String mResourceDir;
	private TexMediaPlayer mMediaPlayer;
	
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
			int width, int height);
	
	private native void nativeDrawRectangle(
			int left, int up, int width, int height, 
			int r, int g, int b,int a);
	
	private native void nativeDrawRectangleChrome(Rect4i rect, Color4i[]color) ;
	
	private native void nativeDrawText(
			String text, int left, int up,
			int r, int g, int b, int a);
	
    private native void nativeSmgrDrawAll();

    
    //native add node API
	private native int nativeAddEmptySceneNode(
			double x, double y, double z, int id, int parent, boolean isLight);
	
	private native int nativeAddCubeSceneNode(
			double x, double y, double z, 
			double size, int id, int parent, boolean isLight);
	
	private native int nativeAddMeshSceneNode(
			String path, double x, double y, double z, 
			int id, int parent, boolean isLight);
	
	private native int nativeAddTextNode(
			String text, double x, double y, double z, 
			double size, int id, int parent, boolean isLight);

	private native int nativeAddCameraSceneNode(
			double px, double py, double pz, 
			double lx, double ly, double lz, 
			boolean isActive, int id, int parent, boolean isLight);
	
	private native int nativeAddBillboardSceneNode(
			double px, double py, double pz, 
			double sx, double sy, int id, int parent, boolean isLight);
	
	private native int nativeAddLightSceneNode(
			double px, double py, double pz, double radius,
			int r, int g, int b, int id, int parent, boolean isLight);
	
	private native int nativeAddAnimateMeshSceneNode(
			String path, double x, double y, double z,
			int id, int parent, boolean isLight);
	
	private native int nativeAddParticleSystemSceneNode(
			double x, double y, double z, boolean withDefaultEmitter, 
			int id, int parent, boolean isLight);
	
	//native remove node
	private native void nativeRemoveNode(int id);
	private native void nativeClear();
	private native void nativeSetFontPath(String path);
	private native int nativeGetMediaTextureId();
}
