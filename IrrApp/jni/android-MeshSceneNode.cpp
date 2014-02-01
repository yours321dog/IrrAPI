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
	void Java_zte_test_irrlib_MeshSceneNode_nativeSetAmbientColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::IMeshSceneNode* node = 
			(IMeshSceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).AmbientColor = video::SColor(a,r,g,b);
	}
	
	void Java_zte_test_irrlib_MeshSceneNode_nativeSetDiffuseColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::IMeshSceneNode* node = 
			(IMeshSceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).DiffuseColor = video::SColor(a,r,g,b);
	}
	
	void Java_zte_test_irrlib_MeshSceneNode_nativeSetEmissiveColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::IMeshSceneNode* node = 
			(IMeshSceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).EmissiveColor = video::SColor(a,r,g,b);
	}
	
	void Java_zte_test_irrlib_MeshSceneNode_nativeSetSpecularColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::IMeshSceneNode* node = 
			(IMeshSceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).SpecularColor = video::SColor(a,r,g,b);
	}
	
	void Java_zte_test_irrlib_MeshSceneNode_nativeSetShininess(
		JNIEnv *env, jobject defaultObj, jdouble shininess, jint materialID, jint id)
	{
		scene::IMeshSceneNode* node = 
			(IMeshSceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).Shininess = (float)shininess;
	}
	
	int Java_zte_test_irrlib_MeshSceneNode_nativeSetTexture(
		JNIEnv *env, jobject defaultObj, jstring path, jint materialID, jint id)
	{		
		const char *msg = env->GetStringUTFChars(path,0);
		core::stringc file = msg;
		
		scene::IMeshSceneNode* node = 
			(IMeshSceneNode*)smgr->getSceneNodeFromId(id);
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
	
	int Java_zte_test_irrlib_MeshSceneNode_nativeAddTextureAnimator(
		JNIEnv *env, jobject defaultObj, jobjectArray array_obj, jint timePerFrame, jboolean loop, jint id)
	{
		scene::IMeshSceneNode* node = 
			(IMeshSceneNode*)smgr->getSceneNodeFromId(id);
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