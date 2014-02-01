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
	void Java_zte_test_irrlib_BillboardSceneNode_nativeSetAmbientColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::IBillboardSceneNode* node = 
			(IBillboardSceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).AmbientColor = video::SColor(a,r,g,b);
	}
	
	void Java_zte_test_irrlib_BillboardSceneNode_nativeSetDiffuseColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::IBillboardSceneNode* node = 
			(IBillboardSceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).DiffuseColor = video::SColor(a,r,g,b);
	}
	
	void Java_zte_test_irrlib_BillboardSceneNode_nativeSetEmissiveColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::IBillboardSceneNode* node = 
			(IBillboardSceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).EmissiveColor = video::SColor(a,r,g,b);
	}
	
	void Java_zte_test_irrlib_BillboardSceneNode_nativeSetSpecularColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::IBillboardSceneNode* node = 
			(IBillboardSceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).SpecularColor = video::SColor(a,r,g,b);
	}
	
	void Java_zte_test_irrlib_BillboardSceneNode_nativeSetShininess(
		JNIEnv *env, jobject defaultObj, jdouble shininess, jint materialID, jint id)
	{
		scene::IBillboardSceneNode* node = 
			(IBillboardSceneNode*)smgr->getSceneNodeFromId(id);
		node->getMaterial(materialID).Shininess = (float)shininess;
	}
	
	int Java_zte_test_irrlib_BillboardSceneNode_nativeSetTexture(
		JNIEnv *env, jobject defaultObj, jstring path, jint materialID, jint id)
	{		
		const char *msg = env->GetStringUTFChars(path,0);
		core::stringc file = msg;
		
		scene::IBillboardSceneNode* node = 
			(IBillboardSceneNode*)smgr->getSceneNodeFromId(id);
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
	
	int Java_zte_test_irrlib_BillboardSceneNode_nativeAddTextureAnimator(
		JNIEnv *env, jobject defaultObj, jobjectArray array_obj, jint timePerFrame, jboolean loop, jint id)
	{
		scene::IBillboardSceneNode* node = 
			(IBillboardSceneNode*)smgr->getSceneNodeFromId(id);
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
	
	/*amazing when used with textures*/
	void Java_zte_test_irrlib_BillboardSceneNode_nativeSetColor(
		JNIEnv *env, jobject defaultObj, jint r1, jint g1, jint b1, jint a1,
		jint r2, jint g2, jint b2, jint a2, jint id)
	{
		scene::IBillboardSceneNode* node = 
			(IBillboardSceneNode*)smgr->getSceneNodeFromId(id);
		 
		video::SColor topColor	=  video::SColor(a1,r1,g1,b1);
		video::SColor bottomColor =  video::SColor(a2,r2,g2,b2);
		node->setColor(topColor, bottomColor);
	}
}