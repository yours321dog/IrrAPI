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

extern "C"
{
	//zhe te me shi shen me yi si? by roy.
	/*amazing when used with textures*/
	void Java_zte_test_irrlib_BillboardSceneNode_nativeSetColor(
		JNIEnv *env, jobject defaultObj, jint r1, jint g1, jint b1, jint a1,
		jint r2, jint g2, jint b2, jint a2, jint id)
	{
		scene::IBillboardSceneNode* node = 
			(IBillboardSceneNode*)smgr->getSceneNodeFromId(id);
		 
		video::SColor topColor	=  video::SColor(a1,r1,g1,b1);
		video::SColor bottomColor =  video::SColor(a2,r2,g2,b2);
		node->setColor(topColor, bottomColor);
	}
}
