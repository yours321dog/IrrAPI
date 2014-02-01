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
	int Java_zte_test_irrlib_AnimateMeshSceneNode_nativeGetStartFrame(
		JNIEnv *env, jobject defaultObj, jint id)
	{
		scene::IAnimatedMeshSceneNode* node = 
			(IAnimatedMeshSceneNode*)smgr->getSceneNodeFromId(id);
		return node->getStartFrame();
	}
	
	int Java_zte_test_irrlib_AnimateMeshSceneNode_nativeGetEndFrame(
		JNIEnv *env, jobject defaultObj, jint id)
	{
		scene::IAnimatedMeshSceneNode* node = 
			(IAnimatedMeshSceneNode*)smgr->getSceneNodeFromId(id);
		return node->getEndFrame();
	}
	
	int Java_zte_test_irrlib_AnimateMeshSceneNode_nativeGetFrameNumber(
		JNIEnv *env, jobject defaultObj, jint id)
	{
		scene::IAnimatedMeshSceneNode* node = 
			(IAnimatedMeshSceneNode*)smgr->getSceneNodeFromId(id);
		return node->getFrameNr();
	}
	
	int Java_zte_test_irrlib_AnimateMeshSceneNode_nativeSetCurrentFrame(
		JNIEnv *env, jobject defaultObj, jint frame, jint id)
	{
		scene::IAnimatedMeshSceneNode* node = 
			(IAnimatedMeshSceneNode*)smgr->getSceneNodeFromId(id);
		
		if(frame<0 || frame >= node->getMesh()->getFrameCount())
			return -1;
		node->setCurrentFrame((float)frame);
		return 0;
	}
	
	void Java_zte_test_irrlib_AnimateMeshSceneNode_nativeSetAnimationSpeed(
		JNIEnv *env, jobject defaultObj, jdouble fps, jint id)
	{
		scene::IAnimatedMeshSceneNode* node = 
			(IAnimatedMeshSceneNode*)smgr->getSceneNodeFromId(id);
		node->setAnimationSpeed(fps);
	}
	
	void Java_zte_test_irrlib_AnimateMeshSceneNode_nativeSetLoopMode(
		JNIEnv *env, jobject defaultObj, jboolean loop, jint id)
	{
		scene::IAnimatedMeshSceneNode* node = 
			(IAnimatedMeshSceneNode*)smgr->getSceneNodeFromId(id);
		node->setLoopMode(loop);
	}
}