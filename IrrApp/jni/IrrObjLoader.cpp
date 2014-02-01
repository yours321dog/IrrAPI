/*Notice:
 irrLoadAnimatedMesh
 LoadTexture
 LoadTextureToMemory
 三个函数是最常用的读取材质的函数，故提供较为方便的接口
 其他不常用的读取材质和模型的函数我们只提供简单接口，具体使用方法请参照irrlicht的例子
  */

#include "IrrObjLoader.h"
using namespace irr;

using namespace os;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/*initiate Loader
@param
	mdevice   - the device pointer  created by IrrMain*/
void IrrObjLoader::initObjLoader(IrrlichtDevice *mdevice)
{
		device = mdevice;
		driver = device->getVideoDriver();
	    smgr = device->getSceneManager();
		guienv = device->getGUIEnvironment();
 }

/*Load Animated Mesh
@param
	meshFileName	-the name of the mesh file
	textureFileName	   - the name of the texture file 
	position	- the position of loaded mesh
	scale	-the scale of the  loaded mesh*/
IAnimatedMeshSceneNode *IrrObjLoader::irrLoadAnimatedMesh(stringc meshFileName,
																		stringc textureFileName,
																		vector3df position, 
																		vector3df scale)
{
	IAnimatedMeshSceneNode *nodeSydney = NULL;
	IAnimatedMesh* mesh = smgr->getMesh( (meshFileName).c_str() );
	        if (!mesh)
	        {
	                device->drop();
					__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "cannot get Mesh");
					return NULL;
	        }
	nodeSydney = smgr->addAnimatedMeshSceneNode( mesh );
	nodeSydney->setPosition(position);
	nodeSydney->setScale(scale);

	if (nodeSydney)
			        {
			                nodeSydney->setMaterialFlag(EMF_LIGHTING, true);
			                //--// no animation
			                //--nodeSydney->setMD2Animation(scene::EMAT_STAND);
			                // with animation
		                    //nodeSydney->setMD2Animation(scene::EMAT_RUN);
			                nodeSydney->setMaterialTexture( 0, driver->getTexture((textureFileName).c_str()) );
			        }else{
			        	__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "cannot add MeshNode");
			        	return NULL;
			        }


	return nodeSydney;

}



IAnimatedMeshSceneNode *IrrObjLoader::irrLoadAnimatedMesh(stringc meshFileName,
																		vector3df position,
																		vector3df scale)
{
	IAnimatedMeshSceneNode *nodeSydney = NULL;
	IAnimatedMesh* mesh = smgr->getMesh( (meshFileName).c_str() );
	        if (!mesh)
	        {
	                device->drop();
					__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "cannot get Mesh");
					return NULL;
	        }
	nodeSydney = smgr->addAnimatedMeshSceneNode( mesh );
	nodeSydney->setPosition(position);
	nodeSydney->setScale(scale);
	return nodeSydney;


}

void IrrObjLoader::LoadTexture(stringc textureFileName,IAnimatedMeshSceneNode *nodeSydney)
{

	if (nodeSydney)
		        {
		                nodeSydney->setMaterialFlag(EMF_LIGHTING, true);
		                //--// no animation
		                //--nodeSydney->setMD2Animation(scene::EMAT_STAND);
		                // with animation
	                    //nodeSydney->setMD2Animation(scene::EMAT_RUN);
		                nodeSydney->setMaterialTexture( 0, driver->getTexture((textureFileName).c_str()) );
		        }else{
		        	__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "cannot add MeshNode");
		        	return;
		        }

}
void IrrObjLoader::LoadTextureToMemory(stringc billTexture)
{
	 driver->getTexture(billTexture);
	    return;

}
