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
	void Java_zte_test_irrlib_SceneNode_nativeSetParent(
		JNIEnv *env, jobject defaultObj, jint parent, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		if(parent == 0) node->setParent(0);
		else {
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node->setParent(parentNode);
		}
	}
	
	void Java_zte_test_irrlib_SceneNode_nativeSetVisible(
		JNIEnv*  env, jobject defaultObj, jboolean isVisible, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		node->setVisible(isVisible);
	}
	
	void Java_zte_test_irrlib_SceneNode_nativeSetRotation(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		node->setRotation(core::vector3df(x,y,z));
	}
	
	void Java_zte_test_irrlib_SceneNode_nativeSetTranslation(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		node->setPosition(core::vector3df(x,y,z));
	}
	
	void Java_zte_test_irrlib_SceneNode_nativeSetScale(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		node->setScale(core::vector3df(x,y,z));
	}
	
	void Java_zte_test_irrlib_SceneNode_nativeSetPosition(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		node->setPosition(core::vector3df(x,y,z));
	}
	
	void Java_zte_test_irrlib_SceneNode_nativeAddRotationAnimator(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		scene::ISceneNodeAnimator* anim = 0;
		anim = smgr->createRotationAnimator (core::vector3df(x,y,z));
		node->addAnimator(anim);
		anim->drop();
	}
	
	void Java_zte_test_irrlib_SceneNode_nativeAddFlyCircleAnimator(
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
	
	void Java_zte_test_irrlib_SceneNode_nativeAddFlyStraightAnimator(
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
	
	void Java_zte_test_irrlib_SceneNode_nativeAddDeleteAnimator(
		JNIEnv *env, jobject defaultObj, jint ms, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		
		scene::ISceneNodeAnimator* anim = 0;
		anim = smgr->createDeleteAnimator(ms);
		node->addAnimator(anim);
		anim->drop();
	}
	
	void Java_zte_test_irrlib_SceneNode_nativeRemoveAllAnimator(
		JNIEnv *env, jobject defaultObj, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		node->removeAnimators();
	}
	
	void Java_zte_test_irrlib_SceneNode_nativeSetTouchable(
		JNIEnv *env, jobject defaultObj, jboolean flag, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		if(!node) return;
		/*create a selector optimized by an octree 
		selector = smgr->createOctreeTriangleSelector(
				node->getMesh(), node, 128);
		*/
		/* for animateSceneNode
		if(flag){
			scene::ITriangleSelector* selector = smgr->createTriangleSelector(node);
			node->setTriangleSelector(selector);
			selector->drop();
		}
		else{
			node->setTriangleSelector(0);
		}
		*/
		/*for meshSceneNode
		if(flag){
			scene::ITriangleSelector* selector = smgr->createTriangleSelector(node->getMesh(),node);
			node->setTriangleSelector(selector);
			selector->drop();
		}
		else{
			node->setTriangleSelector(0);
		}
		*/
		if(flag){
			scene::ITriangleSelector* selector = smgr->createTriangleSelector(((IMeshSceneNode*)node)->getMesh(),node);
			node->setTriangleSelector(selector);
			selector->drop();
		}
		else{
			node->setTriangleSelector(0);
		}
	}

}
