package zte.test.irrlib;

public class NodeFactory {
	//ensure that there is only one instance, so do not use construction method, use getInstance() instead.  
	
	public static NodeFactory getInstance(){
		if (instance == null) 
			instance = new NodeFactory();
		return instance;
	}
	
	private NodeFactory(){
		count = 0;
	}
	
	private static NodeFactory instance = null; 
	
	//main body
	protected int count;
	protected Device mDevice;
	
	public int applyNewID(){
		return ++count;
	}
	
	public static int getNodeID(SceneNode node){
		if (node == null){
			return 0;
		}
		else return node.getID();
	}
	
	public void setIrrDevice(Device dev){
		mDevice = dev;
	}
	
	public void removeNode(SceneNode node){
		unregisterNode(node);
		nativeRemoveNode(node.getID());
	}
	
	void registerNode(SceneNode node){
		mDevice.getScene().registerNode(node);		
	}
	
	boolean unregisterNode(SceneNode node){
		return mDevice.getScene().unregisterNode(node);
	}
	
	public SceneNode addEmptySceneNode(Vector3d pos, SceneNode parent){
		SceneNode node = new SceneNode();
		if (nativeAddEmptySceneNode(pos.x, pos.y, pos.z, getNodeID(node), getNodeID(parent)) != 0)
			return null;
		
		node.setParent(parent);
		node.setPosition(pos);
		node.mark();
		registerNode(node);
		return node;
	}
	
	public MeshSceneNode addCubeSceneNode(Vector3d pos, Vector3d size, SceneNode parent){
		MeshSceneNode node = new MeshSceneNode();
		if (nativeAddCubeSceneNode(pos.x, pos.y, pos.z, size.x, size.y, size.z, getNodeID(node), getNodeID(parent)) != 0)
			return null;
		
		node.setPosition(pos); 
		node.setParent(parent);
		node.mark();
		registerNode(node);
		return node;
	}
	
	public SceneNode addMeshSceneNode(String path, Vector3d pos, SceneNode parent){
		SceneNode node = new SceneNode();
		if (nativeAddMeshSceneNode(mDevice.getPathPrefix() + path, pos.x, pos.y, pos.z, getNodeID(node), getNodeID(parent)) != 0)
			return null;
		
		node.setPosition(pos);
		node.setParent(parent);
		node.mark();
		registerNode(node);
		return node;
	}
	
	public SceneNode addTextNode(String text, Vector3d pos, double size, SceneNode parent){
		SceneNode node = new SceneNode();
		if (nativeAddTextNode(text, pos.x, pos.y, pos.z, size, getNodeID(node), getNodeID(parent)) != 0)
			return null;
		
		node.setPosition(pos);
		node.setParent(parent);
		node.mark();
		registerNode(node);
		return node;
	}
	
	public CameraSceneNode addCameraSceneNode(Vector3d pos, Vector3d lookAt, boolean isActive, SceneNode parent){
		CameraSceneNode node = new CameraSceneNode();
		if (nativeAddCameraSceneNode(pos.x, pos.y, pos.z, lookAt.x, lookAt.y, lookAt.z, isActive, getNodeID(node), getNodeID(parent)) != 0)
			return null;
		
		node.setPosition(pos);
		node.setLookAt(lookAt);
		node.setParent(parent);
		node.mark();
		
		mDevice.getScene().ActiveCamera = node;
		registerNode(node);
		return node;
	}
	
	public BillboardSceneNode addBillboardSceneNode(Vector3d pos, Vector2d size, SceneNode parent){
		BillboardSceneNode node = new BillboardSceneNode();
		if (nativeAddBillboardSceneNode(pos.x, pos.y, pos.z, size.x, size.y, getNodeID(node), getNodeID(parent)) != 0){
			return null;
		}
		
		node.setPosition(pos);
		node.setParent(parent);
		node.mark();
		registerNode(node);
		return node;
	}
	
	public LightSceneNode addLightSceneNode(Vector3d pos, double radius, Color3i color, SceneNode parent){
		LightSceneNode node = new LightSceneNode();
		if (nativeAddLightSceneNode(pos.x, pos.y, pos.z, radius,
				color.r(), color.g(), color.b(), 
				getNodeID(node), getNodeID(parent)) != 0){
			return null;
		}
		
		node.setPosition(pos);
		node.setParent(parent);
		node.mark();
		registerNode(node);
		return node;
	}
	
	public BillboardGroup addBillboardGroup(Vector3d pos, SceneNode parent){
		BillboardGroup node = new BillboardGroup(mDevice.getScene());
		if (nativeAddEmptySceneNode(pos.x, pos.y, pos.z, getNodeID(node), getNodeID(parent)) != 0){
			return null;
		}
		node.setParent(parent);
		node.setPosition(pos);
		node.mark();
		registerNode(node);
		return node;
	}
	
	public AnimateMeshSceneNode addAnimateMeshSceneNode(String path, Vector3d pos, SceneNode parent){
		AnimateMeshSceneNode node = new AnimateMeshSceneNode();
		if (nativeAddAnimateMeshSceneNode(path, pos.x, pos.y, pos.z, getNodeID(node), getNodeID(parent)) != 0){
			return null;
		}
		node.setParent(parent);
		node.setPosition(pos);
		node.mark();
		registerNode(node);
		return node;
	}
	
	public ParticleSystemSceneNode addParticleSystemSceneNode(Vector3d pos, boolean withDefaultEmitter, SceneNode parent){
		ParticleSystemSceneNode node = new ParticleSystemSceneNode();
		if (nativeAddParticleSystemSceneNode(pos.x, pos.y, pos.z, withDefaultEmitter, getNodeID(node), getNodeID(parent)) != 0){
			return null;
		}
		node.setParent(parent);
		node.setPosition(pos);
		node.mark();
		registerNode(node);
		return node;
	}
	
	//return-value 0 means no error
	private native int nativeAddEmptySceneNode(double x, double y, double z, int id, int parent);
	private native int nativeAddCubeSceneNode(double x, double y, double z, double sizex, double sizey, double sizez, int id, int parent);
	private native int nativeAddMeshSceneNode(String path, double x, double y, double z, int id, int parent);
	private native int nativeAddTextNode(String text, double x, double y, double z, double size, int id, int parent);
	
	//modify in 1.24
	private native int nativeAddCameraSceneNode(
			double px, double py, double pz, 
			double lx, double ly, double lz, 
			boolean isActive, int id, int parent);
	//add in 1.24
	private native int nativeAddBillboardSceneNode(
			double px, double py, double pz, 
			double sx, double sy, int id, int parent);
	//add in 1.24
	private native int nativeAddLightSceneNode(
			double px, double py, double pz, double radius,
			int r, int g, int b, int id, int parent);
	//add in 1.25
	private native int nativeAddAnimateMeshSceneNode(String path, double x, double y, double z, int id, int parent);
	//add in 1.25
	private native int nativeAddParticleSystemSceneNode(double x, double y, double z, boolean withDefaultEmitter, int id, int parent);
	//add in 1.25
	private native void nativeRemoveNode(int id);
}
