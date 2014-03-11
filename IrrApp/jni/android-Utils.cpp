#include <jni.h>
#include <irrlicht.h>
#include "android-global.h"
#include <android/log.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "NativeUtils"

extern "C"
{
	void Java_zte_irrlib_Utils_nativeSetSdCardPath( JNIEnv *env, jobject defaultObj, jstring newpath )
	{
		__android_log_print(ANDROID_LOG_INFO, "setsdcardpath", "sdcard setting");
		setgSdCardPath(env,newpath);
	}
}
