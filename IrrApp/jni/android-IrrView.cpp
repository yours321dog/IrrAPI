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
extern stringc gSdCardPath;

extern "C"
{
	void Java_com_ellismarkov_irrEGL2test_IrrView_nativeEnvJ2C(JNIEnv*  env, jobject defaultObj, jstring jsdcardPath)
	{
		const char *msg = env->GetStringUTFChars(jsdcardPath,0);
		gSdCardPath = msg;
		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "not handled %s", gSdCardPath.c_str());
		env->ReleaseStringUTFChars(jsdcardPath,msg);
	}
	
}
	
