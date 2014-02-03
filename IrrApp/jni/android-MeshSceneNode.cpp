#include <jni.h>
#include <android/log.h>
#include <irrlicht.h>
using namespace irr;

using namespace core;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

extern IrrlichtDevice *device;
extern IVideoDriver* driver;
extern ISceneManager* smgr;

extern "C"
{
	void Java_zte_test_irrlib_MeshSceneNode_nativeSetBBoxVisibility(
		JNIEnv *env, jobject defaultObj, jboolean flag)
	{
		/*
		scene::IMeshSceneNode* node = 
			(IMeshSceneNode*)smgr->getSceneNodeFromId(id);
		node->getgetBoundingBox();
		*/
	}
	
}
