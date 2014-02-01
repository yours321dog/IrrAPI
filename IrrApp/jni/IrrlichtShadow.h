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


class IrrlichtShadowDemo
{
private:
	/*
	* Global variables for window size;
	*/
	int gWindowWidth;
	int gWindowHeight;
	stringc gSdCardPath;

	/*
	* Variables of Irrlicht
	*/
	IrrlichtDevice* device;
	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;

	ICameraSceneNode* camera;
	vector3df target;
	vector3df CameraPositon;

	SAppContext context;
	AndroidEventReceiver *receiver;

public:
	/*
	* Constructor and destructor;
	*/
	IrrlichtShadowDemo(){};
	~IrrlichtShadowDemo(){};

	/*
	* Member functions;
	*/
	void iniIrrlichtShadow();
	void update();
	void drop();
	void onScreenSlide(int x, int y);
	void onScreenPinch(int dis);
	void setShadow();
	IrrlichtDevice* getDevice();

};
