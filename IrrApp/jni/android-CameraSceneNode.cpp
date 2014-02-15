#include <jni.h>
#include <irrlicht.h>
#include "android-global.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "NativeCamera"

extern "C"
{
	void Java_zte_irrlib_scene_CameraSceneNode_nativeSetLookAt(
		JNIEnv *env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id)
	{
		scene::ICameraSceneNode* camera = 
			(scene::ICameraSceneNode*)smgr->getSceneNodeFromId(id);
		
		if(camera){
			camera->setTarget(core::vector3df(x,y,z));
		}
		else WARN_NODE_NOT_FOUND(id, SetLookAt);
	}

	void Java_zte_irrlib_scene_CameraSceneNode_nativeSetClipPlane(
		JNIEnv *env, jobject defaultObj, jdouble nearClip, jdouble farClip, jint id)
	{
		scene::ICameraSceneNode* camera =
			(scene::ICameraSceneNode*)smgr->getSceneNodeFromId(id);
		if(camera){
			camera->setNearValue(nearClip);
			camera->setFarValue(farClip);
		}
		else WARN_NODE_NOT_FOUND(id, SetClipPlane);
	}

	void Java_zte_irrlib_scene_CameraSceneNode_nativeSetAspectRatio(
		JNIEnv *env, jobject defaultObj, jdouble ratio, jint id)
	{
		scene::ICameraSceneNode* camera =
			(scene::ICameraSceneNode*)smgr->getSceneNodeFromId(id);
		if(camera){
			camera->setAspectRatio(ratio);
		}
		else WARN_NODE_NOT_FOUND(id, SetAspectRatio);
	}

	void Java_zte_irrlib_scene_CameraSceneNode_nativeSetFovy(
		JNIEnv *env, jobject defaultObj, jdouble fovy, jint id)
	{
		scene::ICameraSceneNode* camera =
			(scene::ICameraSceneNode*)smgr->getSceneNodeFromId(id);
		if(camera){
			camera->setFOV(fovy);
		}
		else WARN_NODE_NOT_FOUND(id, SetFovy);
	}
}