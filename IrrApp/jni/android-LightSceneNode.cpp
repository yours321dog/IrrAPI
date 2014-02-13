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

/* class SLight in SLight.java
public class SLight {
	
	public static final int POINT_LIGHT = 0x01;
	public static final int SPOT_LIGHT = 0x02;
	public static final int DIRECTIONAL_LIGHT = 0x03;
	
	public int Type = POINT_LIGHT;
	
	public Color3i AmbientColor = new Color3i();
	public Color3i DiffuseColor = new Color3i(0xff, 0xff, 0xff);
	public Color3i SpecularColor = new Color3i(0xff, 0xff, 0xff);
	
	//Attenuation factors (constant, linear, quadratic)
	public Vector3d Attenuation = new Vector3d(0, 0, 1.0);
	
	//only available for spot light
	public double OuterCone = 45.0;
	public double InnerCone = 0.0;
	public double Falloff = 2.0;
	
	public Vector3d Direction = new Vector3d();
	public double Radius = 100.0;
}
*/
#define POINT_LIGHT			0x01
#define SPOT_LIGHT			0x02
#define DIRECTIONAL_LIGHT	0x03

extern "C"
{
	void Java_zte_irrlib_scene_LightSceneNode_nativeSendLightData(
		JNIEnv *env, jobject defaultObj, jobject light_obj, jint id)
	{
		jclass light_cls = env->GetObjectClass(light_obj);
		scene::ILightSceneNode *light = 
			(scene::ILightSceneNode *)smgr->getSceneNodeFromId(id);
			
		if(light_cls == NULL || !light)
			return;
		
		jfieldID typeID = env->GetFieldID(light_cls,"Type","I");
		jfieldID outerConeID = env->GetFieldID(light_cls,"OuterCone","D");
		jfieldID innerConeID = env->GetFieldID(light_cls,"InnerCone","D");
		jfieldID fallOffID = env->GetFieldID(light_cls,"Falloff","D");
		jfieldID radiusID = env->GetFieldID(light_cls,"Radius","D");
		
		jint type = (jint)env->GetIntField(light_obj,typeID);
		jdouble outerCone = (jdouble)env->GetDoubleField(light_obj,outerConeID);
		jdouble innerCone = (jdouble)env->GetDoubleField(light_obj,innerConeID);
		jdouble fallOff = (jdouble)env->GetDoubleField(light_obj,fallOffID);
		jdouble radius = (jdouble)env->GetDoubleField(light_obj,radiusID);
		
		switch(type){
			case POINT_LIGHT:
				light->setLightType(video::ELT_POINT);
				break;
			case SPOT_LIGHT:
				light->setLightType(video::ELT_SPOT);
				break;
			case DIRECTIONAL_LIGHT:
				light->setLightType(video::ELT_DIRECTIONAL);
				break;
			default:	
				__android_log_print(ANDROID_LOG_INFO, "FANG", "LightSceneNode: light type error.");
				break;
		}
		light->getLightData().OuterCone = (float)outerCone;
		light->getLightData().InnerCone = (float)innerCone;
		light->getLightData().Falloff = (float)fallOff;
		light->getLightData().Radius = (float)radius;
	}
}

		
		
		
		
