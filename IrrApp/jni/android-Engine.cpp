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
}
