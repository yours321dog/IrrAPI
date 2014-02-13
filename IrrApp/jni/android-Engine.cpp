#include <jni.h>
#include <android/log.h>
#include <irrlicht.h>
#include <importgl.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define TAG "Native Engine"

IrrlichtDevice *device;
IVideoDriver* driver;
ISceneManager* smgr;

int  gWindowWidth  = 100;
int  gWindowHeight = 100;
stringc gSdCardPath = "/sdcard/";

extern video::SColor backColor;
extern "C"
{
	void Java_zte_irrlib_Engine_nativeInit(
		JNIEnv *env, jobject defaultObj)
	{
		importGLInit();
		device = createDevice( video::EDT_OGLES1, dimension2d<u32>(gWindowWidth, gWindowHeight), 16, false, false, false, 0);

		if (device){

			driver = device->getVideoDriver();
			if (!driver){
				__android_log_print(ANDROID_LOG_INFO, TAG, "No driver!");
				return;
			}

			smgr = device->getSceneManager();
			if (!smgr){
				__android_log_print(ANDROID_LOG_INFO, TAG, "No scene manager!");
				return;
			}
			smgr->setAmbientLight(video::SColor(0,250,250,250));
			__android_log_print(ANDROID_LOG_INFO, TAG, "Engine is ready. w: %d, h: %d", gWindowWidth, gWindowHeight);
		}

		__android_log_print(ANDROID_LOG_INFO, TAG, "No device");
	}
	
	void Java_zte_irrlib_Engine_nativeResize(
		JNIEnv *env, jobject defaultObj, jint w, jint h)
	{
		gWindowWidth  = w;
		gWindowHeight = h;
		irr::core::dimension2d<unsigned int> size;
		size.Width = w;
		size.Height = h;
		device->getVideoDriver()->OnResize(size);
	}
	
	void Java_zte_irrlib_Engine_nativeDrop(
		JNIEnv *env, jobject defaultObj)
	{
		device->drop();
	}
	
	void Java_zte_irrlib_Engine_nativeBeginScene(
		JNIEnv *env, jobject defaultObj)
	{
		__android_log_print(ANDROID_LOG_INFO, TAG, "begin scene");
		device->run();
		driver->beginScene(true,true,backColor);
	}
	
	void Java_zte_irrlib_Engine_nativeEndScene(
		JNIEnv *env, jobject defaultObj)
	{

		__android_log_print(ANDROID_LOG_INFO, TAG, "end scene");
		driver->endScene();
	}

	
	double Java_zte_irrlib_Engine_nativeGetFPS(
		JNIEnv *env, jobject defaultObj)
	{
		double fps = (double)driver->getFPS();
		return fps;
	}

	int Java_zte_irrlib_scene_Engine_nativeAddEmptySceneNode(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id, jint parent)
	{
		scene::ISceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addEmptySceneNode(parentNode,id);
		}
		else node = smgr->addEmptySceneNode(0,id);

		if(node) return 0;
		else return -1;
	}

	int Java_zte_irrlib_scene_Engine_nativeAddCubeSceneNode(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z,
		jdouble sizex, jdouble sizey, jdouble sizez, jint id, jint parent)
	{

		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "add cube scene node");
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

	int Java_zte_irrlib_scene_Engine_nativeAddMeshSceneNode(
		JNIEnv*  env, jobject defaultObj, jstring path, jdouble x, jdouble y, jdouble z, jint id, jint parent)
	{
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

	int Java_zte_irrlib_scene_Engine_nativeAddTextNode(
		JNIEnv*  env, jobject defaultObj, jstring text, jdouble x, jdouble y, jdouble z, jdouble size, jint id, jint parent)
	{
		/*Original Func:
		ITextSceneNode* addTextSceneNode(gui::IGUIFont* font, const wchar_t* text,
			video::SColor color=video::SColor(100,255,255,255),
			ISceneNode* parent = 0, const core::vector3df& position = core::vector3df(0,0,0),
			s32 id=-1)
		*/

		/*
		gui::IGUIFont* font = device->getGUIEnvironment()->getFont("/data/data/com.ellismarkov.irrlicht/files/Irrlicht/fonthaettenschweiler.bmp");
		if(!font) font = device->getGUIEnvironment()->getBuiltInFont();
		*/

		core::vector3df pos = core::vector3df(x,y,z);

		scene::ITextSceneNode* node = NULL;

		IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
		if(font){
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

	int Java_zte_irrlib_scene_Engine_nativeAddCameraSceneNode(
		JNIEnv*  env, jobject defaultObj, jdouble px, jdouble py, jdouble pz,
		jdouble lx, jdouble ly, jdouble lz, jint id, jint parent)
	{
		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "add camera scene node");
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

	int Java_zte_irrlib_scene_Engine_nativeAddBillboardSceneNode(
		JNIEnv *env, jobject defaultObj, jdouble px, jdouble py, jdouble pz,
		jdouble sx, jdouble sy, jint id, jint parent)
	{
		core::vector3df pos = core::vector3df(px,py,pz);
		core::dimension2d<f32> size = core::dimension2d<f32>(sx,sy);
		scene::IBillboardSceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addBillboardSceneNode(parentNode,size,pos,id);
		}
		else node = smgr->addBillboardSceneNode(0,size,pos,id);

		if(node) return 0;
		else return -1;
	}

	int Java_zte_irrlib_scene_Engine_nativeAddLightSceneNode(
		JNIEnv *env, jobject defaultObj, jdouble px, jdouble py, jdouble pz,
		jdouble radius, jint r, jint g, jint b, jint id, jint parent)
	{
		core::vector3df pos = core::vector3df(px,py,pz);
		video::SColor color(255,r,g,b);
		scene::ILightSceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addLightSceneNode(parentNode,pos,color,radius,id);
		}
		else node = smgr->addLightSceneNode(0,pos,color,radius,id);

		if(node) return 0;
		else return -1;
	}

	int Java_zte_irrlib_scene_Engine_nativeAddAnimateMeshSceneNode(
		JNIEnv*  env, jobject defaultObj, jstring path, jdouble x, jdouble y, jdouble z, jint id, jint parent)
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

	int Java_zte_irrlib_scene_Engine_nativeAddParticleSystemSceneNode(
		JNIEnv *env, jobject defaultObj, jdouble x, jdouble y, jdouble z,
		jboolean withDefaultEmitter, int id, int parent)
	{
		//1.25: must use default emitter now
		if(!withDefaultEmitter) return -1;

		core::vector3df pos = core::vector3df(x,y,z);
		scene::IParticleSystemSceneNode* ps = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			ps = smgr->addParticleSystemSceneNode(withDefaultEmitter, parentNode, id, pos);
		}
		else ps = smgr->addParticleSystemSceneNode(withDefaultEmitter, 0, id, pos);

		//1.25: use default affector currently
		scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
		ps->addAffector(paf);
		paf->drop();

		if(ps) return 0;
		else return -1;
	}

	void Java_zte_irrlib_scene_Engine_nativeRemoveNode(
		JNIEnv *env, jobject defaultObj, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		scene::ISceneNode* parentNode = node->getParent();
		parentNode->removeChild(node);
	}

}

