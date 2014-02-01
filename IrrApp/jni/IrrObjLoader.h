#include <android/log.h>
#include <irrlicht.h>
#include <os.h>
using namespace irr;

using namespace os;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class IrrObjLoader{
	public:
		IrrlichtDevice *device;
	    IVideoDriver* driver;
	    ISceneManager* smgr;
	    IGUIEnvironment* guienv;
	    
	    IrrObjLoader(){}
	    void initObjLoader(IrrlichtDevice *mdevice);
	    IAnimatedMeshSceneNode *irrLoadAnimatedMesh(stringc sydneyFileName, stringc textureFileName,vector3df position, vector3df scale);
	    IAnimatedMeshSceneNode *irrLoadAnimatedMesh(stringc sydneyFileName,vector3df position, vector3df scale);
	    void LoadTexture(stringc textureFileName,IAnimatedMeshSceneNode *nodeSydney);
	    void LoadTextureToMemory(stringc billTexture);
};
