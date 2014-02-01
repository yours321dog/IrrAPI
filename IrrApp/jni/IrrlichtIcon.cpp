#include "IrrlichtIcon.h"
#include "android-receiver.h"
#include "CSceneManager.h"

using namespace irr;

using namespace os;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

void IrrlichtIconDemo::iniIrrlichtIcon()
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

	device->getFileSystem()->addZipFileArchive("/sdcard/Irrlicht/map-20kdm2.pk3");

	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;
	if (mesh) node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
	node->setPosition(core::vector3df(-1350,-130,-1400));

//	node = smgr->addLightSceneNode(0, core::vector3df(0,0,0),video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 1500.0f);
//	node->setDebugDataVisible ( scene::EDS_BBOX );

	// add fly circle animator to light 1
//	scene::ISceneNodeAnimator* anim  = 0;
//	anim = smgr->createFlyCircleAnimator (core::vector3df(20,200,0),200.0f);
//	node->addAnimator(anim);
//	anim->drop();

	//light
	node = smgr->addLightSceneNode(0, core::vector3df(0,0,0),video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 1500.0f);
	node->setDebugDataVisible ( scene::EDS_BBOX );

	// add fly circle animator to light 1
	scene::ISceneNodeAnimator* anim  = 0;
	anim = smgr->createFlyCircleAnimator (core::vector3df(-75,30,-80),100.0f);
	node->addAnimator(anim);
	anim->drop();

	// attach billboard to the light
	scene::ISceneNode* bill = smgr->addBillboardSceneNode(node, core::dimension2d<f32>(60, 60));

	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture(gSdCardPath+ "/Irrlicht/fireball.bmp"));

	//figure
	scene::IAnimatedMeshSceneNode* anode = 0;
	video::SMaterial material;
	anode = smgr->addAnimatedMeshSceneNode(smgr->getMesh(gSdCardPath + "/Irrlicht/dwarf.x"));
	anode->setPosition(core::vector3df(-70,-66,-80));
	anode->setRotation(core::vector3df(0,-90,0));
	anode->setScale(core::vector3df(1.3,1.3,1.3));
	//anode->getMaterial(0).Lighting = true;
	anode->setAnimationSpeed(20.f);
//	material.setTexture(0, driver->getTexture(gSdCardPath + "/Irrlicht/faerie2.bmp"));
//	//material.Lighting = true;
//	material.NormalizeNormals = true;
//	node->getMaterial(0) = material;

	anode->addShadowVolumeSceneNode();
	smgr->setShadowColor(video::SColor(150,0,0,0));

	target = core::vector3df(-70,30,-60);
	CameraPositon = core::vector3df(70,30,-60);
	camera = smgr->addCameraSceneNode(0, CameraPositon,target);
	camera->bindTargetAndRotation(true);
	camera->setFarValue(10000.0f);

}

void IrrlichtIconDemo::update()
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

void IrrlichtIconDemo::onScreenSlide(int x, int y)
{
	target.rotateXZBy((float)x/30.0, CameraPositon);

	target.Y = target.Y - (float)y/10.0;
	//target.rotateYZBy(y/30, CameraPositon);

	camera->setTarget(target);
	__android_log_print(ANDROID_LOG_INFO, "pos", "target:%d,%d,%d", target.X,target.Y,target.Z);
}

void IrrlichtIconDemo::onScreenPinch(int dis)
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

void IrrlichtIconDemo::drop()
{
	device->drop();
}

void IrrlichtIconDemo::setShadow()
{
	((scene::CSceneManager*)smgr)->setShadowFlag();
}

IrrlichtDevice* IrrlichtIconDemo::getDevice()
{
	return device;
}
