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

	void Java_zte_test_irrlib_SceneNode_nativeSetAmbientColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).AmbientColor = video::SColor(a,r,g,b);
	}

	void Java_zte_test_irrlib_SceneNode_nativeSetDiffuseColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).DiffuseColor = video::SColor(a,r,g,b);
	}

	void Java_zte_test_irrlib_SceneNode_nativeSetEmissiveColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).EmissiveColor = video::SColor(a,r,g,b);
	}

	void Java_zte_test_irrlib_SceneNode_nativeSetSpecularColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).SpecularColor = video::SColor(a,r,g,b);
	}

	void Java_zte_test_irrlib_SceneNode_nativeSetShininess(
		JNIEnv *env, jobject defaultObj, jdouble shininess, jint materialID, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).Shininess = (float)shininess;
	}

	int Java_zte_test_irrlib_SceneNode_nativeSetTexture(
		JNIEnv *env, jobject defaultObj, jstring path, jint materialID, jint id)
	{
		const char *msg = env->GetStringUTFChars(path,0);
		core::stringc file = msg;

		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		if(!node) return -1;
		node->getMaterial(materialID).setTexture(
			0,driver->getTexture(file.c_str()));

		/* another method: set specific layer with new texture for all materials
		if(materialID >= video::MATERIAL_MAX_TEXTURES)
				return -1;
		node->setMaterialTexture(materialID,driver->getTexture(file.c_str()));
		*/
		env->ReleaseStringUTFChars(path,msg);
		return 0;
	}

	int Java_zte_test_irrlib_SceneNode_nativeAddTextureAnimator(
		JNIEnv *env, jobject defaultObj, jobjectArray array_obj, jint timePerFrame, jboolean loop, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		if(!node) return -1;
		int len = env->GetArrayLength(array_obj);
		if(len<=0 || !node) return -1;

		core::array<video::ITexture*> texArr;
		int i = 0;
		core::stringc file;
		for(i=0;i<len;++i){
			jstring path = (jstring)env->GetObjectArrayElement(array_obj,i);
			const char *msg = env->GetStringUTFChars(path,0);
			file = msg;
			texArr.push_back(driver->getTexture(file.c_str()));
			env->ReleaseStringUTFChars(path,msg);
		}

		scene::ISceneNodeAnimator* texAni =
			smgr->createTextureAnimator(texArr, timePerFrame, loop);
		node->addAnimator(texAni);
		return 0;
	}
}
