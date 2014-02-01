#include "IrrlichtShadow.h"
#include "android-receiver.h"
#include "CSceneManager.h"

using namespace irr;

using namespace os;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

void IrrlichtShadowDemo::iniIrrlichtShadow()
{
	//Window Variables
	gSdCardPath = "/sdcard/";
	//high display
//	gWindowWidth = 720;
//	gWindowHeight = 1280;

	//low display
	gWindowWidth = 480;
	gWindowHeight = 800;



	//Irrlicht Variables
	device = createDevice(video::EDT_OGLES1, dimension2d<u32>(gWindowWidth, gWindowHeight), 16, true, true, false, 0);
	driver = device->getVideoDriver();

	if (!device)
	{
		__android_log_print(ANDROID_LOG_INFO, "Enron", "No device");
	}

	if (!driver)
	{
		__android_log_print(ANDROID_LOG_INFO, "Enron", "No driver");
	}

	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	//figure scene
	//Create Room
	stringc roomFileName = "/Irrlicht/room2.3ds";
	stringc wallFileName = "/Irrlicht/wall.jpg";

	IAnimatedMesh* mesh = smgr->getMesh(gSdCardPath + roomFileName);
	if (!mesh)
	{
		//device->drop();
		__android_log_print(ANDROID_LOG_INFO, "Enron", "cannot getMesh");
		return;
	}

	smgr->getMeshManipulator()->makePlanarTextureMapping(mesh->getMesh(0), 0.004f);
	ISceneNode* node = 0;
	node = smgr->addAnimatedMeshSceneNode(mesh);
	node->setMaterialTexture(0, driver->getTexture(gSdCardPath + wallFileName));
	node->getMaterial(0).SpecularColor.set(0,0,0,0);

	//Environmet light
	//node->setMaterialFlag(EMF_LIGHTING, false);



	//Create light
	node = smgr->addLightSceneNode(0, core::vector3df(0,0,0),video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 1500.0f);
	node->setDebugDataVisible ( scene::EDS_BBOX );

	// add fly circle animator to light 1
	scene::ISceneNodeAnimator* anim  = 0;
	anim = smgr->createFlyCircleAnimator (core::vector3df(20,200,0),200.0f);
	node->addAnimator(anim);
	anim->drop();

	// attach billboard to the light
	scene::ISceneNode* bill = smgr->addBillboardSceneNode(node, core::dimension2d<f32>(60, 60));

	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture(gSdCardPath+ "/Irrlicht/fireball.bmp"));

	__android_log_print(ANDROID_LOG_INFO, "Enron", "bill texture");

	// add animated character


	//lengzhui
	scene::IAnimatedMeshSceneNode* anode = 0;
	anode = smgr->addAnimatedMeshSceneNode(smgr->getMesh(gSdCardPath + "/Irrlicht/primitive.3DS"));
	anode->setPosition(core::vector3df(-50,-20,-60));
	anode->setScale(core::vector3df(2,3,2));


	//ninja
//	scene::IAnimatedMeshSceneNode* anode = 0;
//	anode = smgr->addAnimatedMeshSceneNode(smgr->getMesh(gSdCardPath + "/Irrlicht/ninja.b3d"));
//	anode->setPosition(core::vector3df(-50,20,-60));
//	anode->setScale(core::vector3df(13,13,13));
////	anode->getMaterial(0).Lighting = true;
//	anode->setAnimationSpeed(20.f);


	//dwarf

//	stringc dwarfFileName = "/Irrlicht/dwarf.x";
//	mesh = smgr->getMesh(gSdCardPath + dwarfFileName);
//
//	scene::IAnimatedMeshSceneNode* anode = 0;
//
//	anode = smgr->addAnimatedMeshSceneNode(mesh);
//	anode->setPosition(core::vector3df(-50,20,-60));
//	anode->setAnimationSpeed(15);
//	anode->setScale(core::vector3df(2,2,2));


	//Sydney

//	stringc sydneyFileName = "/Irrlicht/sydney.md2";
//	stringc textureFileName = "/Irrlicht/sydney.bmp";
//	mesh = smgr->getMesh(gSdCardPath + sydneyFileName);
//
//	IAnimatedMeshSceneNode* anode = smgr->addAnimatedMeshSceneNode( mesh );
//
//	if (node)
//	{
//		anode->setMaterialFlag(EMF_LIGHTING, false);
//		anode->setPosition(core::vector3df(-50,20,-60));
//	    anode->setMD2Animation(scene::EMAT_STAND);
//	    anode->setMaterialTexture( 0, driver->getTexture(gSdCardPath+textureFileName) );
//	    anode->setScale(core::vector3df(2,2,2));
//	}


	//add shadow
	anode->addShadowVolumeSceneNode();
	smgr->setShadowColor(video::SColor(150,0,0,0));


	//high display
	guienv->addButton(rect<s32>(590,1180,590+120,1180 + 70), 0, GUI_ID_SHADOW, L"Shadow");
	guienv->addButton(rect<s32>(440,1180,440+120,1180 + 70), 0, GUI_ID_QUIT_BUTTON, L"Exit");

	//low display
//	guienv->addButton(rect<s32>(350,700,350+120,700 + 70), 0, GUI_ID_SHADOW, L"Shadow");
//	guienv->addButton(rect<s32>(200,700,200+120,700 + 70), 0, GUI_ID_QUIT_BUTTON, L"Exit");


	//add receiver
	receiver = new AndroidEventReceiver(context);

	// And tell the device to use our custom event receiver.
    device->setEventReceiver(receiver);

    //Add button. Notice: you should reg it in android-reciever.h and you can define it's ability in android-reciever.cpp and irrmain::update()
    context.device = device;
    context.counter = 0;
    context.listbox = 0;

	//figure



	//scene::ICameraSceneNode* camera  smgr->addCameraSceneNode(0, vector3df(0,10,-40), vector3df(0,5,0));
	// vector3df(-50,20,-60)
	target = core::vector3df(-50,20,-60);
	CameraPositon = core::vector3df(150,200,-310);
	camera = smgr->addCameraSceneNode(0, CameraPositon,target);
	camera->bindTargetAndRotation(true);
	camera->setFarValue(10000.0f);

}

void IrrlichtShadowDemo::update()
{
	device->run();

	driver->beginScene(true, true, video::SColor(255,200,200,200));
	smgr->drawAll();
	guienv->drawAll();

	driver->endScene();

	//Print log of FPS
	int fps = driver->getFPS();
	__android_log_print(ANDROID_LOG_INFO, "FPS", "fps=%d", fps);

}

void IrrlichtShadowDemo::onScreenSlide(int x, int y)
{
	target.rotateXZBy((float)x/30.0, CameraPositon);

	target.Y = target.Y - (float)y/10.0;
	//target.rotateYZBy(y/30, CameraPositon);

	camera->setTarget(target);
}

void IrrlichtShadowDemo::onScreenPinch(int dis)
{
	int camX, camY, camZ, diffX, diffY, diffZ;
	double rate = 0;
	camX = camera->getAbsolutePosition().X;
	camY = camera->getAbsolutePosition().Y;
	camZ = camera->getAbsolutePosition().Z;

	diffX = target.X - camX;
	diffY = target.Y - camY;
	diffZ = target.Z - camZ;

	rate = (double)dis/5000000;

	core::vector3df newPos = core::vector3df(camX + diffX *rate, camY + diffY*rate, camZ + diffZ*rate);
	core::vector3df newTar = core::vector3df(target.X + diffX *rate, target.Y + diffY*rate, target.Z + diffZ*rate);
	target = newTar;
	CameraPositon = newPos;

	camera->setPosition(newPos);
	camera->setTarget(newTar);

	__android_log_print(ANDROID_LOG_INFO, "pos", "cam:%d,%d,%d; target:%d,%d,%d", newPos.X,newPos.Y,newPos.Z,newTar.X,newTar.Y,newTar.Z);
}

void IrrlichtShadowDemo::drop()
{
	device->drop();
}

void IrrlichtShadowDemo::setShadow()
{
	((scene::CSceneManager*)smgr)->setShadowFlag();
}

IrrlichtDevice* IrrlichtShadowDemo::getDevice()
{
	return device;
}
