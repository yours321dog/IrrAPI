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
ISceneManager* smgr;

int  gWindowWidth  = 640;               //灞忓箷瀹藉害
int  gWindowHeight = 480;               //灞忓箷楂樺害
stringc gSdCardPath = "/sdcard/";       //SD鍗¤矾寰�

extern video::SColor backColor;
extern "C"
{
	void Java_zte_test_irrlib_Device_nativeInit(
		JNIEnv *env, jobject defaultObj, jint w, jint h)
	{
		importGLInit(); 
		gWindowWidth = w;
		gWindowHeight = h;
		device = createDevice( video::EDT_OGLES1, dimension2d<u32>(gWindowWidth, gWindowHeight), 16, false, false, false, 0);

		
		//杈撳嚭鎻愮ず淇℃伅
		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "createDevice r=%d w=%d h=%d", device, gWindowWidth, gWindowHeight);

		if (!device)
			__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "No device");

		driver = device->getVideoDriver();
		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "getVideoDriver r=%d", driver);
		if (!driver)
		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "No driver");

		smgr = device->getSceneManager();
		if (!smgr)
				__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "No smgr");

		smgr->setAmbientLight(video::SColor(0,250,250,250));
		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "device complete");
	}
	
	void Java_zte_test_irrlib_Device_nativeResize(
		JNIEnv *env, jobject defaultObj, jint w, jint h)
	{
		gWindowWidth  = w;
		gWindowHeight = h;
		irr::core::dimension2d<unsigned int> size;
		size.Width = w;
		size.Height = h;
		device->getVideoDriver()->OnResize(size);
	}
	
	void Java_zte_test_irrlib_Device_nativeDrop(
		JNIEnv *env, jobject defaultObj)
	{
		device->drop();
	}
	
	void Java_zte_test_irrlib_Device_nativeDrawIteration(
		JNIEnv *env, jobject defaultObj)
	{
		nativeDrawIteration();
	/*
		device->run();
		driver->beginScene(true,true,video::SColor(100,100,0,100));
		device->getSceneManager()->drawAll();
		driver->endScene();
	*/
	}
	
	void Java_zte_test_irrlib_Device_nativeBeginScene(
		JNIEnv *env, jobject defaultObj)
	{
		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "begin scene");
		device->run();
		driver->beginScene(true,true,backColor);
	}
	
	void Java_zte_test_irrlib_Device_nativeEndScene(
		JNIEnv *env, jobject defaultObj)
	{

		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "end scene");
		driver->endScene();
	}
	
	void Java_zte_test_irrlib_Device_nativeSetClipPlane(
		JNIEnv *env, jobject defaultObj, jdouble nearClip, jdouble farClip)
	{
		scene::ICameraSceneNode* camera = smgr->getActiveCamera();
		if(camera){
			camera->setNearValue(nearClip);
			camera->setFarValue(farClip);
		}
	}
	
	double Java_zte_test_irrlib_Device_nativeGetFPS(
		JNIEnv *env, jobject defaultObj)
	{
		double fps = (double)driver->getFPS();
		return fps;
	}
}
