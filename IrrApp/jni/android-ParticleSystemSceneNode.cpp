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

/*class Param in ParticleSystemSceneNode.java
public class Param{
		double BBox[];
		Vector3d InitialDirection;
		double MinEmitRate, MaxEmitRate;
		Color4i DarkestColor, BrightestColor;
		double MinLifeTime, MaxLifeTime;
		double MaxAngleDegrees;
		Vector2d MinSize, MaxSize;
	}
*/

extern "C"
{
	void Java_zte_test_irrlib_LightSceneNode_nativeSetEmitter(
		JNIEnv *env, jobject defaultObj, jint type, jobject param_obj, jint id)
	{
		jclass param_cls = env->GetObjectClass(param_obj);
		scene::IParticleSystemSceneNode* ps = 
			(scene::IParticleSystemSceneNode*)smgr->getSceneNodeFromId(id);
		if(param_cls == NULL || !ps) 
			return;
		
		jfieldID bboxID = env->GetFieldID(param_cls,"MinEmitRate","[D");
		jfieldID minRateID = env->GetFieldID(param_cls,"MinEmitRate","D");
		jfieldID maxRateID = env->GetFieldID(param_cls,"MaxEmitRate","D");
		jfieldID minTimeID = env->GetFieldID(param_cls,"MinLifeTime","D");
		jfieldID maxTimeID = env->GetFieldID(param_cls,"MaxLifeTime","D");
		jfieldID maxAngleID = env->GetFieldID(param_cls,"MaxAngleDegrees","D");
		
		jdoubleArray bboxArr = (jdoubleArray)(env->GetObjectField(param_obj,bboxID)); 
		jdouble* bboxVal = env->GetDoubleArrayElements(bboxArr,NULL); 
		jsize len = env->GetArrayLength(bboxArr);
		if(len<5) return;		//no enough elements for bbox
		core::aabbox3d<f32> bbox = core::aabbox3d<f32>(
			bboxVal[0],bboxVal[1],bboxVal[2],bboxVal[3],bboxVal[4],bboxVal[5]);
		env->ReleaseDoubleArrayElements(bboxArr,bboxVal,JNI_ABORT); 
		
		jdouble minRate = (jdouble)env->GetDoubleField(param_obj,minRateID);
		jdouble maxRate = (jdouble)env->GetDoubleField(param_obj,maxRateID);
		jdouble minTime = (jdouble)env->GetDoubleField(param_obj,minTimeID);
		jdouble maxTime = (jdouble)env->GetDoubleField(param_obj,maxTimeID);
		jdouble maxAngle = (jdouble)env->GetDoubleField(param_obj,maxAngleID);
		 
		scene::IParticleEmitter* em = ps->createBoxEmitter(
			bbox, 								// emitter size
			core::vector3df(0.00f,0.05f,0.0f),	// initial direction
			minRate,maxRate,                    // emit rate
			video::SColor(0,255,255,255),       // darkest color
			video::SColor(0,255,255,255),       // brightest color
			minTime,maxTime,maxAngle,           // min and max age, angle
			core::dimension2df(20.f,20.f), 		// min size
			core::dimension2df(20.f,20.f) 		// max size 
		);
		ps->setEmitter(em);
		em->drop();
	}
}