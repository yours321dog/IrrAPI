#include "IrrMain.h"
using namespace irr;

using namespace os;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#include <android-receiver.h>

SAppContext context;
int importGLInit();
void importGLDeinit();
bool aa;

// init IrrMain
void IrrMain::initMain()
{
	gSdCardPath = "/sdcard/";
	gWindowWidth = 480;
	gWindowHeight = 800;
	aa=true;
	importGLInit();


	device = createDevice( video::EDT_OGLES1, dimension2d<u32>(gWindowWidth, gWindowHeight), 32, false, true);
	driver = device -> getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "createDevice r=%d w=%d h=%d", device, gWindowWidth, gWindowHeight);
	__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "getVideoDriver r=%d", driver);
	device->getFileSystem()->changeWorkingDirectoryTo(gSdCardPath+"/irrlicht/");

//	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
//			rect<int>(10,10,260,22), true);

	IAnimatedMesh* mesh = smgr->getMesh("sydney.md2");
	if (!mesh)
	{
		__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "No mesh");
		device->drop();
			//return 1;
	}

	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
	}

	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}

	device->drop();

//
//	device = createDevice( video::EDT_OGLES1, dimension2d<u32>(gWindowWidth, gWindowHeight), 32, false, true);
//    driver = device->getVideoDriver();
//    smgr = device->getSceneManager();
//    guienv = device->getGUIEnvironment();
//    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "testssssssss r=%d w=%d h=%d", device, gWindowWidth, gWindowHeight);
//    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "createDevice r=%d w=%d h=%d", device, gWindowWidth, gWindowHeight);
//    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "createDevice r=%d w=%d h=%d", device, gWindowWidth, gWindowHeight);
//    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "getVideoDriver r=%d", driver);
//
//	device->getFileSystem()->changeWorkingDirectoryTo(gSdCardPath+"/irrlicht/");
//
//	smgr->addCameraSceneNode(0, vector3df(5.0f,5.0f,6.0f), vector3df(0,2,0));
//	// Then create the event receiver, giving it that context structure.
//	//如果你想添加按键，必须有context、且需要在receiver.h里面注册
//	guienv->addButton(rect<s32> (30, 470, 90, 470 + 20), 0, GUI_ID_UP_BUTTON,
//		        			L"WhiteLight");
//	guienv->addButton(rect<s32> (30, 490, 90, 490 + 20), 0, GUI_ID_DOWN_BUTTON,
//			        			L"RedLight");
//	guienv->addButton(rect<s32> (250, 470, 310, 470 + 20), 0, GUI_ID_LEFT_BUTTON,
//			        			L"AddMesh");
//	guienv->addButton(rect<s32> (250, 490, 310, 490 + 20), 0, GUI_ID_RIGHT_BUTTON,
//			        			L"AddTex");
//	guienv->addButton(rect<s32> (140, 470, 200, 470 + 20), 0, GUI_ID_RR_BUTTON,
//				        			L"AddRoom");
//	//guienv->addButton(rect<s32> (140, 490, 200, 490 + 20), 0, GUI_ID_LR_BUTTON,
//	//			        			L"AddWall");
//
//
//    receiver = new AndroidEventReceiver(context);
//
//    // And tell the device to use our custom event receiver.
//    device->setEventReceiver(receiver);
//    //Add button. Notice: you should reg it in android-reciever.h and you can define it's ability in android-reciever.cpp and irrmain::update()
//    context.device = device;
//    context.counter = 0;
//    context.listbox = 0;
}

/* Add Light
@param
	position  -the position of Light ScenNode
	color	   -the color of Light
	radius     -the radius of lighting range
	billTexture  - the texture of Billboard added to Light*/
ILightSceneNode* 	IrrMain:: addLight(vector3df 	position = vector3df(0, 0, 0),  
																SColorf  color = SColorf(1.0f, 1.0f, 1.0f),  
																f32 	radius = 100.0f, 
																stringc billTexture = "")
{
	// add light 1
    scene::ILightSceneNode* light1 = smgr->addLightSceneNode(0, position, color, radius);
    light1->setDebugDataVisible ( scene::EDS_BBOX );

    // add fly circle animator to light 1
    scene::ISceneNodeAnimator* anim =
            smgr->createFlyCircleAnimator (core::vector3df(50,200,0),200.0f);
    light1->addAnimator(anim);
    anim->drop();
    
    // attach billboard to the light
    scene::ISceneNode* bill =
            smgr->addBillboardSceneNode(light1, core::dimension2d<f32>(60, 60));

    bill->setMaterialFlag(video::EMF_LIGHTING, false);
    bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    bill->setMaterialTexture(0, driver->getTexture(billTexture));
    
    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "bill texture");
    return light1;
}



// update IrrMain
void IrrMain::update()
{
	device->run();
	
	// SColor is the background color
	driver->beginScene(true, true, SColor(255,255,0,0));

	smgr->drawAll();
	guienv->drawAll();
	//if(aa==true)
	//	__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "11111111111111111");
	driver->endScene();

	int fps = driver->getFPS();
    //__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "Sydney fps=%d", fps);
}

// drop IrrMain
void IrrMain::drop()
{
	device->drop();
	importGLDeinit();
}
