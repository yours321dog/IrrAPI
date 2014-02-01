#include <jni.h>
#include <android/log.h>
#include <irrlicht.h>
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int importGLInit();
void importGLDeinit();
void nativeDrawIteration();
void initMovie();
//获取当前时间
static long
_getTime(void)
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}

// global variables
extern int  gWindowWidth;               //屏幕宽度
extern int  gWindowHeight;               //屏幕高度
int  gAppAlive   = 1;                   //程序是否存活
extern stringc gSdCardPath;       //SD卡路径
static int sAppStopped = 0;             //程序是否暂停
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;

int dir = 0;
extern IrrlichtDevice *device;
extern IVideoDriver* driver;
extern video::SColor backColor;

/* For JNI: C++ compiler need this */
extern "C" {

	/** Activity onCreate */
	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeOnCreate( JNIEnv*  env )
	{

	}

	/** Activity onPause */
	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeOnPause( JNIEnv*  env )
	{
		sAppStopped = !sAppStopped;                         //暂停一次改变一次，用于判断程序处于暂停或已重启
		//if(sAppStopped) importGLDeinit();
		/*
			if (sAppStopped) {
				sTimeStopped = _getTime();
			} else {
				sTimeOffset -= _getTime() - sTimeStopped;
			}
		  */
	}

	/** Activity onResume */
	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeOnResume( JNIEnv*  env )
	{
		//nativeDrawIteration();
	}

	/** Activity onDestroy */
	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeOnDestroy( JNIEnv*  env )
	{
		importGLDeinit();    //OPENGL 初始化函数
	}

	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeInitGL( JNIEnv*  env )
	{
		importGLInit();     //OPENGL 初始化函数
		sAppStopped = 0;    //程序启动
		sTimeOffsetInit = 0;


		/*
		SIrrlichtCreationParameters Param;
		Param.AntiAlias = 4;
		Param.DriverType = video::EDT_OGLES1;
		Param.WindowSize = core::dimension2d<u32>(gWindowWidth, gWindowHeight);
		Param.Fullscreen = true;
		device = createDeviceEx(Param);
		*/
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

	//依据设备特性重置高和宽
	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
	{

		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "resize w=%d h=%d", w, h);
		gWindowWidth  = w;
		gWindowHeight = h;
		irr::core::dimension2d<unsigned int> size;
		size.Width = w;
		size.Height = h;
		device->getVideoDriver()->OnResize(size);
	}

	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeSendEvent( JNIEnv*  env, jobject defaultObj, jobject event)
	{
		jclass classEvent = env->GetObjectClass(event);
		jfieldID fieldAction = env->GetFieldID(classEvent, "mAction", "I");
		jfieldID fieldX = env->GetFieldID(classEvent, "mX", "F");
		jfieldID fieldY = env->GetFieldID(classEvent, "mY", "F");
		int action = env->GetIntField(event, fieldAction);
		// convert Android coord to OpenGL coords
		float x = env->GetFloatField(event, fieldX);
		float y = env->GetFloatField(event, fieldY);
		SEvent irrevent;
		irrevent.MouseInput.ButtonStates = 0xffffffff;
		irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
		irrevent.MouseInput.Event = irr::EMIE_COUNT;
		irrevent.MouseInput.X = x;
		irrevent.MouseInput.Y = y;
		if (action==0) {
			irrevent.MouseInput.ButtonStates = 0;
			irrevent.MouseInput.ButtonStates = irr::EMBSM_LEFT;
			irrevent.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
			irrevent.MouseInput.ButtonStates ^= irr::EMBSM_LEFT;
			device->postEventFromUser(irrevent);
			__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "left click received %d %f %f", action, x, y);
		} else if (action==1){
			irrevent.MouseInput.ButtonStates = 0;
			irrevent.MouseInput.ButtonStates = irr::EMBSM_LEFT;
			irrevent.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
			irrevent.MouseInput.ButtonStates ^= irr::EMBSM_LEFT;
			device->postEventFromUser(irrevent);
			__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "left click release %d %f %f", action, x, y);
		} else {
			__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "not handled %d", action);
		}
	}

	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeGetStatus(JNIEnv*  env, jobject defaultObj, jobject status)
	{
		if (gAppAlive==0) {
			jclass classStatus = env->GetObjectClass(status);
			jfieldID fieldStatus = env->GetFieldID(classStatus, "mQuit", "Z");
			env->SetBooleanField(status, fieldStatus, true);
		}
	}


	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeEnvJ2C(JNIEnv*  env, jobject defaultObj, jstring jsdcardPath)
	{
		char ligne[1024+1];
		const char *msg = env->GetStringUTFChars(jsdcardPath,0);
		gSdCardPath = msg;
		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "not handled %s", gSdCardPath.c_str());
		env->ReleaseStringUTFChars(jsdcardPath,msg);
	}

	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeDrawIteration( JNIEnv*  env )
	{
		nativeDrawIteration();
	}

	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeSetDirection( JNIEnv*  env, jobject  thiz, jint x)
	{
		//__android_log_print(ANDROID_LOG_INFO, "FANG SETDI", "DIR: %d",x);
		dir = x;
	}
	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeInitMovie( JNIEnv*  env )
	{
		initMovie();
	}

	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeBeginScene( JNIEnv*  env )
	{
		device->run();
		driver->beginScene(true, true, backColor);
	}

	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeEndScene( JNIEnv*  env )
	{
		driver->endScene();
		//device->drop();
	}

	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeSmgrDrawAll( JNIEnv*  env )
	{
		device->getSceneManager()->drawAll();
	}
	void Java_com_ellismarkov_irrEGL2test_IrrlichtTest_nativeGUIDrawAll( JNIEnv*  env )
	{
		device->getGUIEnvironment()->drawAll();
	}
}
