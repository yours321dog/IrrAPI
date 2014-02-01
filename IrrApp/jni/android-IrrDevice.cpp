#include <jni.h>
#include <android/log.h>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// global variables
int importGLInit();
void nativeDrawIteration();

IrrlichtDevice *device;
IVideoDriver* driver;

int  gWindowWidth  = 640;               //屏幕宽度
int  gWindowHeight = 480;               //屏幕高度
stringc gSdCardPath = "/sdcard/";       //SD卡路径

extern video::SColor backColor;
extern "C"
{
	void Java_zte_test_irrlib_IrrDevice_nativeInit(
		JNIEnv *env, jobject defaultObj, jint w, jint h)
	{
		importGLInit(); 
		gWindowWidth = w;
		gWindowHeight = h;
		device = createDevice( video::EDT_OGLES1, dimension2d<u32>(gWindowWidth, gWindowHeight), 16, false, false, false, 0);
		driver = device->getVideoDriver();

		//输出提示信息
		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "createDevice r=%d w=%d h=%d", device, gWindowWidth, gWindowHeight);
		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "getVideoDriver r=%d", driver);

		if (!device)
			__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "No device");
		if (!driver)
			__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "No driver");


		device->getSceneManager()->setAmbientLight(video::SColor(0,250,250,250));
	}
	
	void Java_zte_test_irrlib_IrrDevice_nativeResize(
		JNIEnv *env, jobject defaultObj, jint w, jint h)
	{
		gWindowWidth  = w;
		gWindowHeight = h;
		irr::core::dimension2d<unsigned int> size;
		size.Width = w;
		size.Height = h;
		device->getVideoDriver()->OnResize(size);
	}
	
	void Java_zte_test_irrlib_IrrDevice_nativeDrop(
		JNIEnv *env, jobject defaultObj, jint w, jint h)
	{
		device->drop();
	}

	void Java_zte_test_irrlib_IrrDevice_nativeBeginScene(
		JNIEnv *env, jobject defaultObj)
	{
		driver->beginScene();
	}

	void Java_zte_test_irrlib_IrrDevice_nativeEndScene(
		JNIEnv *env, jobject defaultObj)
	{
		driver->endScene();
	}

	void Java_zte_test_irrlib_IrrDevice_nativeDrawIteration(
		JNIEnv *env, jobject defaultObj){

		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "nativeDrawIteration begin.");
		nativeDrawIteration();
		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "nativeDrawIteration end.");
/*
				device->run();
				driver->beginScene(true,true,video::SColor(100,100,0,100));
				device->getSceneManager()->drawAll();
				driver->endScene();
*/

	}
}
