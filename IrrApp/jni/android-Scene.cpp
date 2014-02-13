#include <jni.h>
#include <android/log.h>
#include <irrlicht.h>
#define TAG "irr-smgr"

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
video::SColor backColor = video::SColor(255,180,180,255);

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
		printf("drawALl");
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

	int Java_zte_irrlib_scene_Scene_nativeAddEmptySceneNode(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z, jint id, jint parent)
	{
		scene::ISceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addEmptySceneNode(parentNode,id);
		}
		else node = smgr->addEmptySceneNode(0,id);

		if(node) return 0;
		else return -1;
	}

	int Java_zte_irrlib_scene_Scene_nativeAddCubeSceneNode(
		JNIEnv*  env, jobject defaultObj, jdouble x, jdouble y, jdouble z,
		jdouble sizex, jdouble sizey, jdouble sizez, jint id, jint parent)
	{

		core::vector3df pos = core::vector3df(x,y,z);
		core::vector3df rot = core::vector3df(0,0,0);
		core::vector3df scale = core::vector3df(sizex,sizey,sizez);

		scene::ISceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addCubeSceneNode(10.0f,parentNode,id,pos,rot,scale);
		}
		else node = smgr->addCubeSceneNode(10.0f,0,id,pos,rot,scale);

		if(node){
			__android_log_print(ANDROID_LOG_INFO, TAG, "add cube scene node");
			node->setMaterialFlag(EMF_LIGHTING, false);
			return 0;
		}
		else return -1;
	}

	int Java_zte_irrlib_scene_Scene_nativeAddMeshSceneNode(
		JNIEnv*  env, jobject defaultObj, jstring path, jdouble x, jdouble y, jdouble z, jint id, jint parent)
	{
		core::vector3df pos = core::vector3df(x,y,z);

		const char *msg = env->GetStringUTFChars(path,0);
		core::stringc meshPath = msg;
		scene::IMesh* mesh = smgr->getMesh(meshPath.c_str());

		scene::IMeshSceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addMeshSceneNode(mesh,parentNode,id,pos);
		}
		else node = smgr->addMeshSceneNode(mesh,0,id,pos);

		if(node) return 0;
		else return -1;
	}

	int Java_zte_irrlib_scene_Scene_nativeAddTextNode(
		JNIEnv*  env, jobject defaultObj, jstring text, jdouble x, jdouble y, jdouble z, jdouble size, jint id, jint parent)
	{
		/*Original Func:
		ITextSceneNode* addTextSceneNode(gui::IGUIFont* font, const wchar_t* text,
			video::SColor color=video::SColor(100,255,255,255),
			ISceneNode* parent = 0, const core::vector3df& position = core::vector3df(0,0,0),
			s32 id=-1)
		*/

		/*
		gui::IGUIFont* font = device->getGUIEnvironment()->getFont("/data/data/com.ellismarkov.irrlicht/files/Irrlicht/fonthaettenschweiler.bmp");
		if(!font) font = device->getGUIEnvironment()->getBuiltInFont();
		*/

		core::vector3df pos = core::vector3df(x,y,z);

		scene::ITextSceneNode* node = NULL;

		IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
		if(font){
			const char *msg = env->GetStringUTFChars(text,0);
			core::stringw strw = msg;
			if(parent != 0){
				scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
				node = smgr->addTextSceneNode(font, strw.c_str(), video::SColor(255,255,255,255),parentNode, pos, id);
			}

			else node = smgr->addTextSceneNode(font, strw.c_str(), video::SColor(255,255,255,255),0, pos, id);
		}
		if(node) return 0;
		else return -1;
	}

	int Java_zte_irrlib_scene_Scene_nativeAddCameraSceneNode(
		JNIEnv*  env, jobject defaultObj, jdouble px, jdouble py, jdouble pz,
		jdouble lx, jdouble ly, jdouble lz, jint id, jint parent)
	{
		core::vector3df pos = core::vector3df(px,py,pz);
		core::vector3df lookat = core::vector3df(lx,ly,lz);
		__android_log_print(ANDROID_LOG_INFO, TAG, "camera node added? id: %d, parent: %d", id, parent);

		scene::ICameraSceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addCameraSceneNode(parentNode,pos,lookat,id);
		}
		else node = smgr->addCameraSceneNode(0,pos,lookat,id);
		
		if(node){
			__android_log_print(ANDROID_LOG_INFO, TAG, "camera node added. id: %d", node->getID());
			return 0;
		}
		else return -1;
	}

	int Java_zte_irrlib_scene_Scene_nativeAddBillboardSceneNode(
		JNIEnv *env, jobject defaultObj, jdouble px, jdouble py, jdouble pz,
		jdouble sx, jdouble sy, jint id, jint parent)
	{
		core::vector3df pos = core::vector3df(px,py,pz);
		core::dimension2d<f32> size = core::dimension2d<f32>(sx,sy);
		scene::IBillboardSceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addBillboardSceneNode(parentNode,size,pos,id);
		}
		else node = smgr->addBillboardSceneNode(0,size,pos,id);

		if(node) return 0;
		else return -1;
	}

	int Java_zte_irrlib_scene_Scene_nativeAddLightSceneNode(
		JNIEnv *env, jobject defaultObj, jdouble px, jdouble py, jdouble pz,
		jdouble radius, jint r, jint g, jint b, jint id, jint parent)
	{
		core::vector3df pos = core::vector3df(px,py,pz);
		video::SColor color(255,r,g,b);
		scene::ILightSceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addLightSceneNode(parentNode,pos,color,radius,id);
		}
		else node = smgr->addLightSceneNode(0,pos,color,radius,id);

		if(node) return 0;
		else return -1;
	}

	int Java_zte_irrlib_scene_Scene_nativeAddAnimateMeshSceneNode(
		JNIEnv*  env, jobject defaultObj, jstring path, jdouble x, jdouble y, jdouble z, jint id, jint parent)
	{
		ISceneManager* smgr = device->getSceneManager();
		core::vector3df pos = core::vector3df(x,y,z);

		const char *msg = env->GetStringUTFChars(path,0);
		core::stringc meshPath = msg;
		scene::IAnimatedMesh* mesh = smgr->getMesh(meshPath.c_str());

		scene::IAnimatedMeshSceneNode* node = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			node = smgr->addAnimatedMeshSceneNode(mesh,parentNode,id,pos);
		}
		else node = smgr->addAnimatedMeshSceneNode(mesh,0,id,pos);

		if(node) return 0;
		else return -1;
	}

	int Java_zte_irrlib_scene_Scene_nativeAddParticleSystemSceneNode(
		JNIEnv *env, jobject defaultObj, jdouble x, jdouble y, jdouble z,
		jboolean withDefaultEmitter, int id, int parent)
	{
		//1.25: must use default emitter now
		if(!withDefaultEmitter) return -1;

		core::vector3df pos = core::vector3df(x,y,z);
		scene::IParticleSystemSceneNode* ps = NULL;
		if(parent != 0){
			scene::ISceneNode* parentNode = smgr->getSceneNodeFromId(parent);
			ps = smgr->addParticleSystemSceneNode(withDefaultEmitter, parentNode, id, pos);
		}
		else ps = smgr->addParticleSystemSceneNode(withDefaultEmitter, 0, id, pos);

		//1.25: use default affector currently
		scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
		ps->addAffector(paf);
		paf->drop();

		if(ps) return 0;
		else return -1;
	}

	void Java_zte_irrlib_scene_Scene_nativeRemoveNode(
		JNIEnv *env, jobject defaultObj, jint id)
	{
		scene::ISceneNode* node = smgr->getSceneNodeFromId(id);
		scene::ISceneNode* parentNode = node->getParent();
		parentNode->removeChild(node);
	}

	void Java_zte_irrlib_scene_Scene_nativeClear(JNIEnv *env, jobject defaultObj){
		if (smgr){
			smgr->clear();
			__android_log_print(ANDROID_LOG_INFO, TAG, "All Node Cleared!");
		}
	}
}
	
	
