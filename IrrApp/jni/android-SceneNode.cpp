#include <jni.h>
#include <irrlicht.h>
#include "android-global.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "NativeNode"

extern "C"
{
	int Java_zte_irrlib_scene_SceneNode_nativeSetParent(
		JNIEnv *env, jobject defaultObj, jint parent, jint id)
	{
		//LOGD("PARENT %d", id);
		ISceneNode *node = smgr->getSceneNodeFromId(id),
			*parentNode = NULL;
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetParent);
			return -1;
		}
		
		if (parent == 0)
		{
			node->setParent(smgr->getRootSceneNode());
		}
		else 
		{
			parentNode = smgr->getSceneNodeFromId(parent);
			if (!parentNode)
			{
				WARN_PARENT_NOT_FOUND(id, SetParent);
				return -1;
			}
			node->setParent(parentNode);
		}
		return 0;
	}
	
	int Java_zte_irrlib_scene_SceneNode_nativeSetVisible(
		JNIEnv*  env, jobject defaultObj, jboolean isVisible, jint id)
	{
		ISceneNode* node = smgr->getSceneNodeFromId(id);
		//LOGD("VISIBLE %d", id);
		if (!node) 
		{
			WARN_NODE_NOT_FOUND(id, SetVisible);
			return -1;
		}
		node->setVisible(isVisible);
		return 0;
	}
	
	int Java_zte_irrlib_scene_SceneNode_nativeSetRotation(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id)
	{
		ISceneNode* node = smgr->getSceneNodeFromId(id);
		//LOGD("ROTAION %d", id);
		if (!node) 
		{
			WARN_NODE_NOT_FOUND(id, SetRotation);
			return -1;
		}
		node->setRotation(core::vector3df(x,y,z));
		return 0;
	}
	
	int Java_zte_irrlib_scene_SceneNode_nativeSetScale(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id)
	{
		//LOGD("SCALE %d", id);
		ISceneNode* node = smgr->getSceneNodeFromId(id);
		if (!node) 
		{
			WARN_NODE_NOT_FOUND(id, SetScale);
			return -1;
		}
		node->setScale(core::vector3df(x,y,z));
		return 0;
	}
	
	int Java_zte_irrlib_scene_SceneNode_nativeSetPosition(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id)
	{
		ISceneNode* node = smgr->getSceneNodeFromId(id);
		//LOGD("POSITION %d", id);
		if (!node) 
		{
			WARN_NODE_NOT_FOUND(id, SetPosition);
			return -1;
		}
		node->setPosition(core::vector3df(x,y,z));
		return 0;
	}
	
	void Java_zte_irrlib_scene_SceneNode_nativeAddRotationAnimator(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		scene::ISceneNodeAnimator* anim = 0;
		anim = smgr->createRotationAnimator (core::vector3df(x,y,z));
		node->addAnimator(anim);
		anim->drop();
	}
	
	void Java_zte_irrlib_scene_SceneNode_nativeAddFlyCircleAnimator(
		JNIEnv*  env, jobject defaultObj, jdouble cx, jdouble cy, jdouble cz,
		jdouble radius, jdouble speed, jdouble ax, jdouble ay, jdouble az, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		
		core::vector3df center = core::vector3df(cx,cy,cz);
		core::vector3df dir = core::vector3df(ax, ay, az);

		scene::ISceneNodeAnimator* anim = 0;
		anim = smgr->createFlyCircleAnimator (center,radius,speed,dir);
		node->addAnimator(anim);
		anim->drop();
	}
	
	void Java_zte_irrlib_scene_SceneNode_nativeAddFlyStraightAnimator(
		JNIEnv*  env, jobject defaultObj, jdouble sx, jdouble sy, jdouble sz,
		jdouble dx, jdouble dy, jdouble dz, jdouble time, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		
		core::vector3df start = core::vector3df(sx,sy,sz);
		core::vector3df end = core::vector3df(dx, dy, dz);
		
		scene::ISceneNodeAnimator* anim = 0;
		anim = smgr->createFlyStraightAnimator (start,end,time);
		node->addAnimator(anim);
		anim->drop();
	}
	
	void Java_zte_irrlib_scene_SceneNode_nativeAddDeleteAnimator(
		JNIEnv *env, jobject defaultObj, jint ms, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		
		scene::ISceneNodeAnimator* anim = 0;
		anim = smgr->createDeleteAnimator(ms);
		node->addAnimator(anim);
		anim->drop();
	}
	
	void Java_zte_irrlib_scene_SceneNode_nativeRemoveAllAnimator(
		JNIEnv *env, jobject defaultObj, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		node->removeAnimators();
	}
}
