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
//ISceneManager* smgr = NULL;

extern "C"
{
	void Java_zte_test_irrlib_IrrSceneNode_nativeSetParent(
		JNIEnv*  env, jobject defaultObj, jint parent, jint id)
	{
		ISceneManager* smgr = device->getSceneManager();
		scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		node->setParent(parentNode);
	}
	
	void Java_zte_test_irrlib_IrrSceneNode_nativeSetVisible(
		JNIEnv*  env, jobject defaultObj, jboolean isVisible, jint id)
	{
		ISceneManager* smgr = device->getSceneManager();
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		node->setVisible(isVisible);
	}
	
	void Java_zte_test_irrlib_IrrSceneNode_nativeSetRotation(
		JNIEnv*  env, jobject defaultObj, jfloat x, jfloat y, jfloat z, jint id)
	{
		ISceneManager* smgr = device->getSceneManager();
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		node->setRotation(core::vector3df(x,y,z));
	}
	
	void Java_zte_test_irrlib_IrrSceneNode_nativeSetScale(
		JNIEnv*  env, jobject defaultObj, jfloat x, jfloat y, jfloat z, jint id)
	{
		ISceneManager* smgr = device->getSceneManager();
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		node->setScale(core::vector3df(x,y,z));
	}
	
	void Java_zte_test_irrlib_IrrSceneNode_nativeSetPosition(
		JNIEnv*  env, jobject defaultObj, jfloat x, jfloat y, jfloat z, jint id)
	{
		ISceneManager* smgr = device->getSceneManager();
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		node->setPosition(core::vector3df(x,y,z));
	}
	
	void Java_zte_test_irrlib_IrrSceneNode_nativeSetTranslate(
		JNIEnv*  env, jobject defaultObj, jfloat x, jfloat y, jfloat z, jint id)
	{
		ISceneManager* smgr = device->getSceneManager();
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		core::vector3df dis = node->getPosition();
		node->setPosition(core::vector3df(dis.X+x, dis.Y+y, dis.Z+z));
	}
	
	void Java_zte_test_irrlib_IrrSceneNode_nativeAddRotationAnimator(
		JNIEnv*  env, jobject defaultObj, jfloat x, jfloat y, jfloat z, jint id)
	{
		ISceneManager* smgr = device->getSceneManager();
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		scene::ISceneNodeAnimator* anim = 0;
		anim = smgr->createRotationAnimator (core::vector3df(x,y,z));
		node->addAnimator(anim);
		anim->drop();
	}
	
	void Java_zte_test_irrlib_IrrSceneNode_nativeAddFlyCircleAnimator(
		JNIEnv*  env, jobject defaultObj, jfloat cx, jfloat cy, jfloat cz,
		jfloat radius, jfloat speed, jfloat ax, jfloat ay, jfloat az, jint id)
	{
		ISceneManager* smgr = device->getSceneManager();
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		
		core::vector3df center = core::vector3df(cx,cy,cz);
		core::vector3df dir = core::vector3df(ax, ay, az);

		scene::ISceneNodeAnimator* anim = 0;
		anim = smgr->createFlyCircleAnimator (center,radius,speed,dir);
		node->addAnimator(anim);
		anim->drop();
	}
	
	void Java_zte_test_irrlib_IrrSceneNode_nativeAddFlyStraightAnimator(
		JNIEnv*  env, jobject defaultObj, jfloat sx, jfloat sy, jfloat sz,
		jfloat dx, jfloat dy, jfloat dz, jfloat time, jint id)
	{
		ISceneManager* smgr = device->getSceneManager();
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		
		core::vector3df start = core::vector3df(sx,sy,sz);
		core::vector3df end = core::vector3df(dx, dy, dz);
		
		scene::ISceneNodeAnimator* anim = 0;
		anim = smgr->createFlyStraightAnimator (start,end,time);
		node->addAnimator(anim);
		anim->drop();
	}
	
	void Java_zte_test_irrlib_IrrSceneNode_nativeSetTexture(
		JNIEnv*  env, jobject defaultObj, jstring path, jint id)
	{
		const char *msg = env->GetStringUTFChars(path,0);
		core::stringc file = msg;
		scene::ISceneNode* node = device->getSceneManager()->getSceneNodeFromId(id);

		node->setMaterialTexture(0,driver->getTexture(file.c_str()));
		env->ReleaseStringUTFChars(path,msg);
	}
	
}
	
