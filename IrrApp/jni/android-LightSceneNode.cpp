#include <jni.h>
#include <irrlicht.h>
#include "android-global.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "NativeLight"

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
	public Vector3d Attenuation = new Vector3d(0, 0, 1);
	
	//only available for spot light
	public double OuterCone = 45.0;
	public double InnerCone = 0.0;
	public double Falloff = 2.0;
	
	public Vector3d Direction = new Vector3d(0, 0, 1);
	public double Radius = 100.0;
}
*/
#define POINT_LIGHT			0x01
#define SPOT_LIGHT			0x02
#define DIRECTIONAL_LIGHT	0x03

#define NUM 10

extern "C"
{
	
	static jfieldID id_field[NUM];
	static bool isInitLight = false;
	
	static void initFieldIdLight(JNIEnv *env, jobject light_obj)
	{
		if (isInitLight) return;
		
		char **name = new char*[NUM];
		char **type = new char*[NUM];
		for (int i = 0; i < NUM; i++)
		{
			name[i] = new char[32];
			type[i] = new char[32];
		}
		
		strcpy(name[0], "Type"); strcpy(type[0], "I");
		strcpy(name[1], "AmbientColor"); strcpy(type[1], "Lzte/irrlib/core/Color3i;");
		strcpy(name[2], "DiffuseColor"); strcpy(type[2], "Lzte/irrlib/core/Color3i;");
		strcpy(name[3], "SpecularColor"); strcpy(type[3], "Lzte/irrlib/core/Color3i;");
		strcpy(name[4], "Attenuation"); strcpy(type[4], "Lzte/irrlib/core/Vector3d;");
		strcpy(name[5], "OuterCone"); strcpy(type[5], "D");
		strcpy(name[6], "InnerCone"); strcpy(type[6], "D");
		strcpy(name[7], "Falloff"); strcpy(type[7], "D");
		strcpy(name[8], "Direction"); strcpy(type[8], "Lzte/irrlib/core/Vector3d;");
		strcpy(name[9], "Radius"); strcpy(type[9], "D");
		
		jclass cls_light = env->GetObjectClass(light_obj);
		for (int i = 0; i < NUM; i++)
			id_field[i] =  env->GetFieldID(cls_light, name[i], type[i]);
		
		for (int i = 0; i < NUM; i++)
		{
			delete [] name[i];
			delete [] type[i];
		}
		delete name, type;
		
		isInitLight = true;
	}	
	
	int Java_zte_irrlib_scene_LightSceneNode_nativeSendLightData(
		JNIEnv *env, jobject defaultObj, jobject light_obj, jint id)
	{
		initFieldIdLight(env, light_obj);
		
		ILightSceneNode *light = 
			(scene::ILightSceneNode *)smgr->getSceneNodeFromId(id);
		
		if (!light)
		{
			WARN_NODE_NOT_FOUND(id, SendLightData);
			return -1;
		}
			
		switch(env->GetIntField(light_obj, id_field[0])){
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
				LOGE("Light type error!");
				return -5;
				break;
		}
		
		SLight &lightData = light->getLightData();
		
		lightData.AmbientColor = createSColorfFromColor3i(env, env->GetObjectField(light_obj, id_field[1]));
		lightData.DiffuseColor = createSColorfFromColor3i(env, env->GetObjectField(light_obj, id_field[2]));
		lightData.SpecularColor = createSColorfFromColor3i(env, env->GetObjectField(light_obj, id_field[3]));
		lightData.Attenuation = createvector3dfFromVector3d(env, env->GetObjectField(light_obj, id_field[4]));
		lightData.OuterCone =  (f32)env->GetDoubleField(light_obj, id_field[5]);
		lightData.InnerCone = (f32)env->GetDoubleField(light_obj, id_field[6]);
		lightData.Falloff = (f32)env->GetDoubleField(light_obj, id_field[7]);
		lightData.Direction = createvector3dfFromVector3d(env, env->GetObjectField(light_obj, id_field[8]));
		lightData.Radius = (f32)env->GetDoubleField(light_obj, id_field[9]);
		
/*		LOGE("%f, %f, %f", lightData.AmbientColor.r, lightData.AmbientColor.g, lightData.AmbientColor.b);
		LOGE("%f, %f, %f", lightData.DiffuseColor.r, lightData.DiffuseColor.g, lightData.DiffuseColor.b);
		LOGE("%f, %f, %f", lightData.SpecularColor.r, lightData.SpecularColor.g, lightData.SpecularColor.b);
		LOGE("%f, %f, %f", lightData.Attenuation.X, lightData.Attenuation.Y, lightData.Attenuation.Z);
		LOGE("%f, %f, %f", lightData.Direction.X, lightData.Direction.Y, lightData.Direction.Z);
		LOGE("%f, %f, %f, %f", lightData.InnerCone, lightData.OuterCone, lightData.Falloff, lightData.Radius);
*/
	}
	
	int Java_zte_irrlib_scene_LightSceneNode_nativeGetLightData(
		JNIEnv *env, jobject defaultObj, jobject light_obj, jint id)
	{
		initFieldIdLight(env, light_obj);
		
		ILightSceneNode *light = 
			(scene::ILightSceneNode *)smgr->getSceneNodeFromId(id);
		
		if (!light)
		{
			WARN_NODE_NOT_FOUND(id, SendLightData);
			return -1;
		}
		
		switch(light->getLightType()){
			case video::ELT_POINT:
				env->SetIntField(light_obj, id_field[0], POINT_LIGHT);
				break;
			case video::ELT_SPOT:
				env->SetIntField(light_obj, id_field[0], SPOT_LIGHT);
				break;
			case video::ELT_DIRECTIONAL:
				env->SetIntField(light_obj, id_field[0], DIRECTIONAL_LIGHT);
				break;
			default:
				LOGE("Light type error!");
				return -5;
				break;
		}
		
		SLight &lightData = light->getLightData();
		
		setColor3iFromSColorf(env, light_obj, id_field[1], lightData.AmbientColor);
		setColor3iFromSColorf(env, light_obj, id_field[2], lightData.DiffuseColor);
		setColor3iFromSColorf(env, light_obj, id_field[3], lightData.SpecularColor);
		setVector3dFromvector3df(env, light_obj, id_field[4], lightData.Attenuation);
		env->SetDoubleField(light_obj, id_field[5], lightData.OuterCone);
		env->SetDoubleField(light_obj, id_field[6], lightData.InnerCone);
		env->SetDoubleField(light_obj, id_field[7], lightData.Falloff);
		setVector3dFromvector3df(env, light_obj, id_field[8], lightData.Direction);
		env->SetDoubleField(light_obj, id_field[9], lightData.Radius);
		
/*		LOGE("%f, %f, %f", lightData.AmbientColor.r, lightData.AmbientColor.g, lightData.AmbientColor.b);
		LOGE("%f, %f, %f", lightData.DiffuseColor.r, lightData.DiffuseColor.g, lightData.DiffuseColor.b);
		LOGE("%f, %f, %f", lightData.SpecularColor.r, lightData.SpecularColor.g, lightData.SpecularColor.b);
		LOGE("%f, %f, %f", lightData.Attenuation.X, lightData.Attenuation.Y, lightData.Attenuation.Z);
		LOGE("%f, %f, %f", lightData.Direction.X, lightData.Direction.Y, lightData.Direction.Z);
		LOGE("%f, %f, %f, %f", lightData.InnerCone, lightData.OuterCone, lightData.Falloff, lightData.Radius);
*/
	}
}

		
		
		
		
