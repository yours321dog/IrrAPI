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
void initMovie();

extern int  gWindowWidth;               //屏幕宽度
extern int  gWindowHeight;               //屏幕高度
extern video::SColor backColor;

extern IrrlichtDevice *device;
extern IVideoDriver* driver;
extern video::SColor backColor;

extern "C"
{
	void Java_com_ellismarkov_irrEGL2test_IrrRenderer_nativeResize(
		JNIEnv*  env, jobject defaultObj, jint w, jint h)
	{
		gWindowWidth  = w;
		gWindowHeight = h;
		irr::core::dimension2d<unsigned int> size;
		size.Width = w;
		size.Height = h;
		device->getVideoDriver()->OnResize(size);
	}
	
	void Java_com_ellismarkov_irrEGL2test_IrrRenderer_nativeInitGL(JNIEnv*)
	{
		importGLInit(); 
		device = createDevice( video::EDT_OGLES2, dimension2d<u32>(gWindowWidth, gWindowHeight), 16, false, false, false, 0);
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
	
	void Java_com_ellismarkov_irrEGL2test_IrrRenderer_nativeDrawIteration(JNIEnv*  env)
	{
		nativeDrawIteration();
		/*
		device->run();
		driver->beginScene(true,true,backColor);
		device->getSceneManager()->drawAll();
		driver->endScene();
		*/
	}

	void Java_com_ellismarkov_irrEGL2test_IrrRenderer_nativeInitMovie( JNIEnv*  env )
	{
		initMovie();
	}

	void Java_com_ellismarkov_irrEGL2test_IrrRenderer_nativeBeginScene( JNIEnv*  env )
	{
		device->run();
		driver->beginScene(true, true, backColor);
	}

	void Java_com_ellismarkov_irrEGL2test_IrrRenderer_nativeEndScene( JNIEnv*  env )
	{
		driver->endScene();
		//device->drop();
	}

	void Java_com_ellismarkov_irrEGL2test_IrrRenderer_nativeSmgrDrawAll( JNIEnv*  env )
	{
		device->getSceneManager()->drawAll();
	}
	void Java_com_ellismarkov_irrEGL2test_IrrRenderer_nativeGUIDrawAll( JNIEnv*  env )
	{
		device->getGUIEnvironment()->drawAll();
	}
	
}
