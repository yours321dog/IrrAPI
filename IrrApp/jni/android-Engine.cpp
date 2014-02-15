#include <jni.h>
#include <irrlicht.h>
#include "android-global.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "NativeEngine"

extern "C"
{
	int Java_zte_irrlib_Engine_nativeInit(
		JNIEnv *env, jobject defaultObj)
	{
		importGLInit();
		
		if (device) 
		{
			delete device;
			device = 0;
			driver = 0;
			smgr = 0;
			collMgr = 0;
		}
		
		device = createDevice( video::EDT_OGLES1, 
			dimension2d<u32>(gWindowWidth, gWindowHeight), 16, false, false, false, 0);

		if (!device)
		{
			LOGE("No device!");
			return -1;
		}
		
		driver = device->getVideoDriver();
		if (!driver){
			LOGE("No driver!"); 
			return -2;
		}

		smgr = device->getSceneManager();
		if (!smgr){
			LOGE("No scene manager!");
			return -3;
		}
		
		smgr->setAmbientLight(video::SColor(0,250,250,250));
		LOGI("Engine is ready. width: %d, height: %d", gWindowWidth, gWindowHeight);
		_isInit = true;
		return 0;
	}
	
	bool Java_zte_irrlib_Engine_nativeIsInit(
		JNIEnv *env, jobject defaultObj)
	{
		return _isInit;
	}
	
	int Java_zte_irrlib_Engine_nativeClear(
		JNIEnv *env, jobject defaultObj)
	{
		//if (device) delete device;
		//device = 0;
		//if (smgr) smgr->clear();
		_isInit = false;
	}
	
	void Java_zte_irrlib_Engine_nativeResize(
		JNIEnv *env, jobject defaultObj, jint w, jint h)
	{
		gWindowWidth  = w;
		gWindowHeight = h;
		dimension2d<u32> size(w, h);
		if (driver)
		{
			driver->OnResize(size);
			smgr->getActiveCamera()->setAspectRatio((double)w/h);
			LOGI("Render size changed, width: %d height: %d", w, h);
		}
		else LOGE("Driver is not ready.");
	}
	
	/*void Java_zte_irrlib_Engine_nativeDrop(
		JNIEnv *env, jobject defaultObj)
	{
		device->drop();
	}*/
	
	void Java_zte_irrlib_Engine_nativeBeginScene(
		JNIEnv *env, jobject defaultObj)
	{
		if (device)	device->run();
		if (driver) driver->beginScene(true,true,backColor);
	}
	
	void Java_zte_irrlib_Engine_nativeEndScene(
		JNIEnv *env, jobject defaultObj)
	{
		if (driver) driver->endScene();
	}

	
	double Java_zte_irrlib_Engine_nativeGetFPS(
		JNIEnv *env, jobject defaultObj)
	{
		if (!driver) return 0.0;
		return (double)(driver->getFPS());
	}
}