// Copyright (C) 2002-2009 Laurent Mallet & Maxime Biais
// This file is part of the "Irrlicht Android Port".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>
#include <android-receiver.h>
#include <android-logger.h>
#include <importgl.h>
#include <instancedMesh.h>
#include <irrlicht.h>
#include <os.h>

using namespace irr;

using namespace os;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;












extern IrrlichtDevice *device;
extern IVideoDriver* driver;
extern stringc gSdCardPath;


ISceneManager* smgr = NULL;
IGUIEnvironment* guienv = NULL;
IMesh* mesh = NULL;
ISceneNode* node = NULL;
IAnimatedMeshSceneNode *nodeSydney = NULL;
ICameraSceneNode* camera = NULL;
IGUIStaticText* diagnostics = NULL;
SAppContext context;
AndroidEventReceiver *receiver;

IAnimatedMeshSceneNode* dwarf;
IShadowVolumeSceneNode* shadow;
//#define USE_INSTANCE
//#define USE_SHADOWMAP


static long _getTime(void)
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}


void initInstancedDraw()
{
	smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    
	__android_log_print(ANDROID_LOG_INFO, "InstancedDraw", "start");
    
    stringc cubefile = "/media/cube.obj";
	mesh = smgr->getMesh( (gSdCardPath+cubefile).c_str() )->getMesh(0);
	 if (!mesh)
	        {
				
	                device->drop();
					__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "cannot getMesh");
					return;
	        }

	
	stringc cubetex = "/media/grass.png";
	mesh->getMeshBuffer(0)->getMaterial().setTexture(0, driver->getTexture((gSdCardPath+cubetex).c_str()));
	
	 __android_log_print(ANDROID_LOG_INFO, "InstancedDraw", "grass material set success");
#ifdef USE_INSTANCE
	InstancedMesh* BatchingMesh = new InstancedMesh();

	 __android_log_print(ANDROID_LOG_INFO, "InstancedDraw", "instanced construct");
#endif
	s32 x,y,z;
	// let's add a grassy floor, 20x20
	for (x=-10; x<11; ++x)
		for (z=-10; z<11; ++z)
			if ( abs(x) > 5 || abs(z) > 5)
			{
				#ifdef USE_INSTANCE
					BatchingMesh->addMesh(mesh, core::vector3df(x*2,-4,z*2));
				#else
					ISceneNode* node = smgr->addMeshSceneNode(mesh, 0, 0, core::vector3df(x*2,-4,z*2));
					node->setMaterialFlag(EMF_LIGHTING, false);
				#endif
			}
				
	
	stringc stonetex = "/media/stone.png";
	mesh->getMeshBuffer(0)->getMaterial().setTexture(0, driver->getTexture((gSdCardPath+stonetex).c_str()));
	 __android_log_print(ANDROID_LOG_INFO, "InstancedDraw", "stone material set success");
	
	// add some walls
	for (y=-1; y<7; ++y)
		for (x=-5; x<6; ++x)
			for (z=-5; z<6; ++z)
				if (((abs(z) == 5 || abs(x) == 5) && (abs(x)>1 || y > 3 )) || y==6)
				{
					#ifdef USE_INSTANCE
						BatchingMesh->addMesh(mesh, core::vector3df(x*2,y*2-2,z*2));
					#else
						ISceneNode* node = smgr->addMeshSceneNode(mesh, 0, 0, core::vector3df(x*2,y*2-2,z*2));
						node->setMaterialFlag(EMF_LIGHTING, false);

					#endif
				}
					
	stringc tiletex = "/media/tile.png";
	mesh->getMeshBuffer(0)->getMaterial().setTexture(0,driver->getTexture((gSdCardPath+tiletex).c_str()));
	 __android_log_print(ANDROID_LOG_INFO, "InstancedDraw", "tile material set success");
	// a nice tiled interior
	for (x=-5; x<6; ++x)
		for (z=-5; z<6; ++z)
		{
			#ifdef USE_INSTANCE
				BatchingMesh->addMesh(mesh, core::vector3df(x*2,-4,z*2));
			#else
				ISceneNode* node = smgr->addMeshSceneNode(mesh, 0, 0, core::vector3df(x*2,-4,z*2));
				node->setMaterialFlag(EMF_LIGHTING, false);
			#endif
		}
			
#ifdef USE_INSTANCE
	BatchingMesh->finalize();
	BatchingMesh->setMaterialFlag(EMF_LIGHTING, false);

	ISceneNode *node = smgr->addOctreeSceneNode(BatchingMesh);
	node->setScale(core::vector3df(10,10,10));
	node->setDebugDataVisible(EDS_OFF);
#endif
	/*
	To be able to look at and move around in this scene, 
	we create a first person shooter style camera and make the 
	mouse cursor invisible.
	*/
	camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 100.0f);
	__android_log_print(ANDROID_LOG_INFO, "InstancedDraw", "camera set success");
	//device->getCursorControl()->setVisible(false);
	__android_log_print(ANDROID_LOG_INFO, "InstancedDraw", "cursor set success");

	
	// Then create the event receiver, giving it that context structure.
    receiver = new AndroidEventReceiver(context);

    // And tell the device to use our custom event receiver.
    device->setEventReceiver(receiver);

	
}


void initEffect2()
{
	smgr = device->getSceneManager();

	// Create a basic fps camera.
	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.05f);
	cam->setPosition(vector3df(2.0f, 5.0f, -5.0f));
	cam->setFarValue(40.0f);
	cam->setNearValue(0.1f);
	
	
	// Load a basic room mesh.
	IMeshSceneNode* room = smgr->addMeshSceneNode(smgr->getMesh("/sdcard/media/ShadRoom.b3d")->getMesh(0));
	room->setScale(vector3df(3.0f, 2.0f, 3.0f));
	room->setPosition(vector3df(4.5f, 0.5f, 4.0f));
	room->setMaterialTexture(0, driver->getTexture("/sdcard/media/wall.jpg"));
	
	room->getMaterial(0).Lighting = false;


#ifdef USE_SHADOWMAP

	EffectHandler* effect = new EffectHandler(device, driver->getScreenSize(), false, true);

	effect->addShadowToNode(room, EFT_4PCF, ESM_RECEIVE);

	// Set a global ambient color. A very dark gray.
	effect->setAmbientColor(SColor(255, 64, 64, 64));
#endif
	// Load the dwarf mesh.
	IAnimatedMesh* dwarfmesh = smgr->getMesh("/sdcard/media/dwarf.x");
	 __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "Effect get mesh");
	// We create 4 dwarves arranged in a grid in a nested loop.
	for(int g = 0;g < 1;g++)
	{
		for(int v = 0;v < 1;v++)
		{
			// Add a dwarf animated mesh scene node.
			dwarf = smgr->addAnimatedMeshSceneNode(dwarfmesh);
			dwarf->setScale(vector3df(0.05f, 0.05f, 0.05f));
			dwarf->setPosition(vector3df(g * 4.5f + 1.0f, 0.5f, v * 3.5f + 2.0f));

			// Disable the lighting here too, XEffects overlay will handle it.
			for(u32 i = 0;i < dwarf->getMaterialCount();++i)
				dwarf->getMaterial(i).Lighting = false;

			dwarf->setAnimationSpeed(20.0f);

#ifdef USE_SHADOWMAP
			effect->addShadowToNode(dwarf, EFT_4PCF);
#else
	shadow = dwarf->addShadowVolumeSceneNode();
#endif
		}
	}


#ifdef USE_SHADOWMAP
	effect->addShadowLight(SShadowLight(512, core::vector3df(37,20,24), vector3df(0, 0, 0), 
		video::SColorf(1.0f,1.0f,1.0f,1.0f)));
#else

		scene::ILightSceneNode * light = smgr->addLightSceneNode(0, core::vector3df(37,20,24),
		video::SColorf(0.8f,0.8f,0.8f), 60.0f);
#endif

}

static int counter = 0;

void nativeDrawIterationInstancedDraw()
{	
     device->run();
    if (counter==0) {
        initInstancedDraw();
    }
	
	// SColor is the background color
	driver->beginScene(true, true, SColor(255,0,0,0));
	__android_log_print(ANDROID_LOG_INFO, "InstancedDraw", "begine scene");
	smgr->drawAll();
	__android_log_print(ANDROID_LOG_INFO, "InstancedDraw", "draw all success");
	guienv->drawAll();

	driver->endScene();

	counter ++ ;


	int fps = driver->getFPS();
    __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "fps=%d", fps);

	stringw str = L"FPS:";
	str += fps;
	guienv->clear();

	  guienv->addButton(rect<s32>(10,10,110,10 + 32), 0, GUI_ID_QUIT_BUTTON,
                            str.c_str(), L"Exits Program");

} 

s32 oldFps = 0;
void nativeDrawIterationEffect2()
{
	device->run();
	  if (counter==0) {
      initEffect2();
  }

		// Get the shadow lights and set their positions to the positions
		// of the light scene nodes.
//		effect->getShadowLight(0).setPosition(light->getPosition());
//		effect->getShadowLight(1).setPosition(light2->getPosition());
//
//		effect->getShadowLight(0).setTarget(vector3df(5, 0, 5));
//		effect->getShadowLight(1).setTarget(vector3df(5, 0, 5));

		driver->beginScene(true, true, SColor(255,255,0,0));

		// EffectHandler->update() replaces smgr->drawAll(). It handles all
		// of the shadow maps, render targets switching, post processing, etc.
		//effect->update();
		 smgr->drawAll();
		driver->endScene();
			
			counter ++ ;
}

void nativeDrawIteration()
{
    nativeDrawIterationEffect2();
	
}

