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
	int Java_zte_test_irrlib_IrrNodeFactory_nativeAddEmptySceneNode(
		JNIEnv*  env, jobject defaultObj, jfloat x, jfloat y, jfloat z, jint id, jint parent)
	{
		ISceneManager* smgr = device->getSceneManager();
		scene::ISceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addEmptySceneNode(parentNode,id);
		}
		else node = smgr->addEmptySceneNode(0,id);


		if(node) return 0;
		else return -1;
	}
	
	int Java_zte_test_irrlib_IrrNodeFactory_nativeAddCubeSceneNode(
		JNIEnv*  env, jobject defaultObj, jfloat x, jfloat y, jfloat z, 
		jfloat sizex, jfloat sizey, jfloat sizez, jint id, jint parent)
	{
		ISceneManager* smgr = device->getSceneManager();
		core::vector3df pos = core::vector3df(x,y,z);
		core::vector3df rot = core::vector3df(0,0,0);
		core::vector3df scale = core::vector3df(sizex,sizey,sizez);
		
		scene::ISceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addCubeSceneNode(10.0f,parentNode,id,pos,rot,scale);
		}
		else node = smgr->addCubeSceneNode(10.0f,0,id,pos,rot,scale);


		
		if(node) return 0;
		else return -1;
	}
	
	int Java_zte_test_irrlib_IrrNodeFactory_nativeAddMeshSceneNode(
		JNIEnv*  env, jobject defaultObj, jstring path, jfloat x, jfloat y, jfloat z, jint id, jint parent)
	{
		ISceneManager* smgr = device->getSceneManager();
		core::vector3df pos = core::vector3df(x,y,z);
		
		const char *msg = env->GetStringUTFChars(path,0);
		core::stringc meshPath = msg;
		scene::IMesh* mesh = smgr->getMesh(meshPath.c_str());

		scene::IMeshSceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addMeshSceneNode(mesh,parentNode,id,pos);
		}
		else node = smgr->addMeshSceneNode(mesh,0,id,pos);
		
		if(node) return 0;
		else return -1;
	}
	
	int Java_zte_test_irrlib_IrrNodeFactory_nativeAddAnimatedSceneNode(
		JNIEnv*  env, jobject defaultObj, jstring path, jfloat x, jfloat y, jfloat z, jint id, jint parent)
	{
		ISceneManager* smgr = device->getSceneManager();
		core::vector3df pos = core::vector3df(x,y,z);
		
		const char *msg = env->GetStringUTFChars(path,0);
		core::stringc meshPath = msg;
		scene::IAnimatedMesh* mesh = smgr->getMesh(meshPath.c_str());

		scene::IAnimatedMeshSceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addAnimatedMeshSceneNode(mesh,parentNode,id,pos);
		}
		else node = smgr->addAnimatedMeshSceneNode(mesh,0,id,pos);
		
		if(node) return 0;
		else return -1;
	}
	
	int Java_zte_test_irrlib_IrrNodeFactory_nativeAddTextNode(
		JNIEnv*  env, jobject defaultObj, jstring text, jfloat x, jfloat y, jfloat z, jfloat size, jint id, jint parent)
	{
		/*
		ITextSceneNode* addTextSceneNode(gui::IGUIFont* font, const wchar_t* text,
			video::SColor color=video::SColor(100,255,255,255),
			ISceneNode* parent = 0, const core::vector3df& position = core::vector3df(0,0,0),
			s32 id=-1)
		*/
		
		ISceneManager* smgr = device->getSceneManager();
		core::vector3df pos = core::vector3df(x,y,z);
		
		scene::ITextSceneNode* node = NULL;
		
		gui::IGUIFont* font = device->getGUIEnvironment()->getFont("/data/data/com.ellismarkov.irrlicht/files/Irrlicht/fonthaettenschweiler.bmp");
		if(!font) font = device->getGUIEnvironment()->getBuiltInFont();
		if(font){
			//const char *msg = env->GetStringChars(text,0);
			const char *msg = env->GetStringUTFChars(text,0);
			core::stringw strw = msg;
			if(parent != 0){
				scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
				node = smgr->addTextSceneNode(font, strw.c_str(), video::SColor(255,255,255,255),parentNode, pos, id);
			}
			
			else node = smgr->addTextSceneNode(font, strw.c_str(), video::SColor(255,255,255,255),0, pos, id);
		}
		if(node) return 0;
		else return -1;
	}
	
	int Java_zte_test_irrlib_IrrNodeFactory_nativeAddCameraSceneNode(
		JNIEnv*  env, jobject defaultObj, jfloat px, jfloat py, jfloat pz, 
		jfloat lx, jfloat ly, jfloat lz, jint id, jint parent)
	{
		ISceneManager* smgr = device->getSceneManager();
		core::vector3df pos = core::vector3df(px,py,pz);
		core::vector3df lookat = core::vector3df(lx,ly,lz);
		
		scene::ICameraSceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addCameraSceneNode(parentNode,pos,lookat,id);
		}
		else node = smgr->addCameraSceneNode(0,pos,lookat,id);

		if(node) return 0;
		else return -1;
	}

}
