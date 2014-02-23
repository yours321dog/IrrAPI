#include <jni.h>
#include <irrlicht.h>
#include "android-global.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "NativeMeshNode"

extern "C"
{
	int Java_zte_irrlib_scene_MeshSceneNode_nativeSetBBoxVisibility(
		JNIEnv *env, jobject defaultObj, jboolean flag, jint id)
	{
		IMeshSceneNode* node = 
			(IMeshSceneNode*)smgr->getSceneNodeFromId(id);
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetBBoxVisibility);
			return -1;
		}
		node->setDebugDataVisible(flag);
		return 0;
	}
	
	int Java_zte_irrlib_scene_MeshSceneNode_nativeSetTouchable(
		JNIEnv *env, jobject defaultObj, jboolean flag, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetTouchable);
			return -1;
		}
		
		if(flag){
			scene::ITriangleSelector* selector = smgr->createTriangleSelector(((IMeshSceneNode*)node)->getMesh(),node);
			node->setTriangleSelector(selector);
			selector->drop();
		}
		else{
			node->setTriangleSelector(0);
		}
		return 0;
	}

	int Java_zte_irrlib_scene_MeshSceneNode_nativeSetSmoothShade(
		JNIEnv *env, jobject defaultObj,jboolean flag, jint materialID, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetSmoothShade);
			return -1;
		}
		node->getMaterial(materialID).GouraudShading = flag;
		return 0;
	}

	
	int Java_zte_irrlib_scene_MeshSceneNode_nativeSetAmbientColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetAmbientColor);
			return -1;
		}		
		node->getMaterial(materialID).AmbientColor = video::SColor(a,r,g,b);
		return 0;
	}

	int Java_zte_irrlib_scene_MeshSceneNode_nativeSetDiffuseColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetDiffuseColor);
			return -1;
		}		
		node->getMaterial(materialID).DiffuseColor = video::SColor(a,r,g,b);
		return 0;
	}

	int Java_zte_irrlib_scene_MeshSceneNode_nativeSetEmissiveColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetEmissiveColor);
			return -1;
		}			
		node->getMaterial(materialID).EmissiveColor = video::SColor(a,r,g,b);
		return 0;
	}

	int Java_zte_irrlib_scene_MeshSceneNode_nativeSetSpecularColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a, jint materialID, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetSpecularColor);
			return -1;
		}				
		node->getMaterial(materialID).SpecularColor = video::SColor(a,r,g,b);
		return 0;
	}

	int Java_zte_irrlib_scene_MeshSceneNode_nativeSetShininess(
		JNIEnv *env, jobject defaultObj, jdouble shininess, jint materialID, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetShininess);
			return -1;
		}	
		node->getMaterial(materialID).Shininess = (float)shininess;
		return 0;
	}

	int Java_zte_irrlib_scene_MeshSceneNode_nativeSetTexture(
		JNIEnv *env, jobject defaultObj, jstring path, jint materialID, jint id)
	{
		const char *msg = env->GetStringUTFChars(path,0);
		core::stringc file = msg;

		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetTexture);
			return -1;
		}	
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
	
	int Java_zte_irrlib_scene_MeshSceneNode_nativeSetBitmapTexture(
		JNIEnv *env, jobject defaultObj, jstring jname, 
		jobject jbitmap, jint materialID, jint id)
	{
		const char *name = env->GetStringUTFChars(jname,0);
		IImage *image = createImageFromBitmap(env, jbitmap);
		
		if (!image)
			return -1;
		
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
			
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetBitmapTexture);
			return -1;
		}	
			
		ITexture *tex = driver->addTexture(name, image, 0);
		node->setMaterialTexture(0, tex);
		env->ReleaseStringUTFChars(jname,name);
		return 0;
	}

	int Java_zte_irrlib_scene_MeshSceneNode_nativeAddTextureAnimator(
		JNIEnv *env, jobject defaultObj, jobjectArray array_obj, jint timePerFrame, jboolean loop, jint id)
	{
		scene::ISceneNode* node =
			(ISceneNode*)smgr->getSceneNodeFromId(id);
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, AddTextureAnimator);
			return -1;
		}	
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
	
	
	
	int Java_zte_irrlib_scene_MeshSceneNode_nativeSetMediaTexture(
		JNIEnv *env, jobject defaultObj, jint mId, jint id)
	{
		ISceneNode* node = smgr->getSceneNodeFromId(id);
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetMediaTexture);
			return -1;
		}	
		node->setMaterialTexture(mId, _extTex);
		return 0;
	}
	
	int Java_zte_irrlib_scene_MeshSceneNode_nativeGetMaterialCount(
		JNIEnv *env, jobject defaultObj, jint id)
	{
		ISceneNode* node = smgr->getSceneNodeFromId(id);
		if (!node)
		{
			WARN_NODE_NOT_FOUND(id, SetMediaTexture);
			return -1;
		}	
		return node->getMaterialCount();
	}
}
