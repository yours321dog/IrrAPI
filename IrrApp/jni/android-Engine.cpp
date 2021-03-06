#include <jni.h>
#include <irrlicht.h>
#include "android-global.h"
#include <string.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "NativeEngine"

extern "C"
{
	int Java_zte_irrlib_Engine_nativeInit(
		JNIEnv *env, jobject defaultObj, int type,
		jobject vector, jobject color4, jobject color3, jobject rect)
	{
		initJNIInfo(env, vector, color4, color3, rect);
		
		video::E_DRIVER_TYPE videoType =  video::EDT_NULL;
		if (type == 0x00000001) videoType = video::EDT_OGLES1;
		else if (type == 0x00000004) videoType = video::EDT_OGLES2;
		
		//importGLInit();
		
		if (device) 
		{
			delete device;
			device = 0;
			driver = 0;
			smgr = 0;
		}
		
		device = createDevice( videoType, 
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
		
		smgr->setAmbientLight(video::SColor(0xff,0x3f,0x3f,0x3f));
		LOGI("Engine is ready. width: %d, height: %d", gWindowWidth, gWindowHeight);
		_isInit = true;
		return 0;
	}
	
	void Java_zte_irrlib_Engine_nativeSetFontPath(
		JNIEnv *env, jobject defaultObj, jstring path)
	{
		const char* text = env->GetStringUTFChars(path, 0);
		strcpy(_builtInFontPath, text);
		env->ReleaseStringUTFChars(path, text);
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