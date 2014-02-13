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
extern ISceneManager* smgr;
scene::ISceneCollisionManager *collMgr;
video::SColor backColor = video::SColor(100,100,100,100);

extern "C"
{
	void Java_zte_irrlib_scene_Scene_nativeSetClearColor(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a)
	{
		backColor = video::SColor(a,r,g,b);
	}
	
	void Java_zte_irrlib_scene_Scene_nativeDrawImage(
		JNIEnv *env, jobject defaultObj, jstring path, jint left, jint up, 
		jint width, jint height, jint transparent)
	{
		const char *msg = env->GetStringUTFChars(path,0);
		core::stringc imagePath = msg;
		video::ITexture* images = driver->getTexture(imagePath.c_str());
		core::dimension2d<u32> size = images->getOriginalSize();
		video::SColor *color = new video::SColor(transparent,255,255,255);

		driver->enableMaterial2D();
		driver->draw2DImage(images, core::rect<s32>(left,up,left+width,up+height),
				core::rect<s32>(0,0,size.Width,size.Height), 0 ,color, true);

		driver->enableMaterial2D(false);
	}
	
	void Java_zte_irrlib_scene_Scene_nativeDrawRectangle(
		JNIEnv *env, jobject defaultObj, jint left, jint up, jint width, jint height, 
		jint r, jint g, jint b, jint transparent)
	{
		driver->draw2DRectangle(video::SColor(transparent,r,g,b),
				core::rect<s32>(left, up, left+width, up+height));
	}
	
	void Java_zte_irrlib_scene_Scene_nativeDrawText(
		JNIEnv *env, jobject defaultObj,jstring text, jint left, jint up, jdouble size, jint r, jint g, jint b, jint a)
	{
	/*
		gui::IGUIFont* font = device->getGUIEnvironment()->getFont("/data/data/com.ellismarkov.irrlicht/files/Irrlicht/fonthaettenschweiler.bmp");
		if(!font) font = device->getGUIEnvironment()->getBuiltInFont();
		*/
		gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
		if (font){
			const char *msg = env->GetStringUTFChars(text,0);
			core::stringc str = msg;
			font->draw(str,core::rect<s32>(left,up,left+size*100,up+size),video::SColor(a,r,g,b));
		}
	}
	
	void Java_zte_irrlib_scene_Scene_nativeSmgrDrawAll(
		JNIEnv *env, jobject defaultObj)
	{
		smgr->drawAll();
	}
	
	void Java_zte_irrlib_scene_Scene_nativeSetAmbientLight(
		JNIEnv *env, jobject defaultObj, jint r, jint g, jint b, jint a)
	{
		smgr->setAmbientLight(video::SColor(a,r,g,b));
	}
	
	void Java_zte_irrlib_scene_Scene_nativeSetActiveCamera(
		JNIEnv *env, jobject defaultObj, jint id)
	{
		scene::ISceneNode* camera = smgr->getSceneNodeFromId(id);
		smgr->setActiveCamera((ICameraSceneNode *)camera);
	}
	
	int Java_zte_irrlib_scene_Scene_nativeGetTouchedSceneNode(
		JNIEnv *env, jobject defaultObj, jint x, jint y, jint root)
	{
		scene::ISceneNode* rootNode = smgr->getSceneNodeFromId(root);
		scene::ICameraSceneNode* camera = smgr->getActiveCamera();
		if(!camera) return -1;
		
		collMgr = smgr->getSceneCollisionManager();
		core::line3d<f32> ray;
		ray = collMgr->getRayFromScreenCoordinates(
            irr::core::position2di(x, y), camera); 
		core::vector3df intersection; 
		core::triangle3df hitTriangle;  
		scene::ISceneNode * selectedSceneNode =
		collMgr->getSceneNodeAndCollisionPointFromRay(
			ray,
			intersection,	//intersection position
			hitTriangle,	//hit triangle
			0,				//idBitMask: 0 to test all nodes
			rootNode		//root node to search from
		);
		if(selectedSceneNode)
			return selectedSceneNode->getID();
		else return -1;
	}
}
	
	
