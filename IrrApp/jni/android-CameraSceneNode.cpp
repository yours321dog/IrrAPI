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
	void Java_zte_test_irrlib_CameraSceneNode_nativeSetLookAt(
		JNIEnv *env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id)
	{
		scene::ICameraSceneNode* camera = 
			(scene::ICameraSceneNode*)smgr->getSceneNodeFromId(id);
		
		//camera->updateAbsolutePosition();
		//camera->bindTargetAndRotation(ture);
		camera->setTarget(core::vector3df(x,y,z));
	}
}


void Java_zte_test_irrlib_CameraSceneNode_nativeSetClipPlane(
	JNIEnv *env, jobject defaultObj, jdouble nearClip, jdouble farClip, jint id)
{
	scene::ICameraSceneNode* camera =
		(scene::ICameraSceneNode*)smgr->getSceneNodeFromId(id);
	if(camera){
		camera->setNearValue(nearClip);
		camera->setFarValue(farClip);
	}
}
