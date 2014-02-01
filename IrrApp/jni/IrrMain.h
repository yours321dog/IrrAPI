#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>
#include <iostream>
#include <android-receiver.h>
#include <android-logger.h>
#include <importgl.h>

#include <irrlicht.h>
#include <os.h>
using namespace irr;

using namespace os;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class IrrMain{
	public:
		// global variables
		int  gWindowWidth;
		int  gWindowHeight;
		stringc gSdCardPath;
		

		IrrlichtDevice *device;
	    IVideoDriver* driver;
	    ISceneManager* smgr;
	    IGUIEnvironment* guienv;
	    
	    SAppContext context;
	    AndroidEventReceiver *receiver;
	    
	    IrrMain(){};
	    void initMain();
	    ILightSceneNode* addLight(vector3df position,  SColorf color,  f32 radius, stringc billTexture);
	    void addButton();
	    void update();
	    void drop();
	   // void LoadTextureToMemory(stringc billTexture);
};
