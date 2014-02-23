#include <sys/time.h>
#include "android-global.h"
#include <android/bitmap.h>
#include "COGLESTexture.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device = 0;
IVideoDriver* driver = 0;
ISceneManager* smgr = 0;

int gWindowWidth = 640;
int gWindowHeight = 480;
stringc gSdCardPath = "/sdcard/";
SColor backColor = SColor(255,180,180,255);

bool _isInit = false;
char _extPrefix[] = "<external>";
char _builtInFontPath[] = "/sdcard/irrmedia/bigfont.png";
ITexture *_extTex = 0;

static jclass cls_color4i;
static jfieldID id_red, id_green, id_blue, id_alpha;

static jclass cls_color3i;
static jfieldID id_red3, id_green3, id_blue3;

static jclass cls_rect4i;
static jfieldID id_left, id_top, id_right, id_bottom;

static jclass cls_vector3d;
static jfieldID id_vx, id_vy, id_vz;

void setVector3dFromvector3df(JNIEnv *env, jobject light, jfieldID id, const vector3df& vec)
{
	jobject obj = env->GetObjectField(light, id);
	env->SetDoubleField(obj, id_vx, vec.X);
	env->SetDoubleField(obj, id_vy, vec.Y);
	env->SetDoubleField(obj, id_vz, vec.Z);
	LOGD("%f, %f, %f", vec.X, vec.Y, vec.Z);
}

void setColor3iFromSColorf(JNIEnv *env, jobject light, jfieldID id, const SColorf& color)
{
	jobject obj = env->GetObjectField(light, id);
	env->SetIntField(obj, id_red3, color.r*255);
	env->SetIntField(obj, id_green3, color.g*255);
	env->SetIntField(obj, id_blue3, color.b*255);
}

SColorf createSColorfFromColor3i(JNIEnv *env, jobject color)
{
	return SColorf(env->GetIntField(color, id_red3)/255.0, env->GetIntField(color, id_green3)/255.0,
			 env->GetIntField(color, id_blue3)/255.0, 1.0);
}

SColor createSColorFromColor3i(JNIEnv *env, jobject color)
{
	return SColor(0xff, env->GetIntField(color, id_red), 
			env->GetIntField(color, id_green), env->GetIntField(color, id_blue));
}

SColor createSColorFromColor4i(JNIEnv *env, jobject color)
{
	return SColor(env->GetIntField(color, id_alpha), env->GetIntField(color, id_red), 
			env->GetIntField(color, id_green), env->GetIntField(color, id_blue));
}

recti createrectiFromRect4i(JNIEnv *env, jobject rec)
{
	return recti(env->GetIntField(rec, id_left), env->GetIntField(rec, id_top),
			env->GetIntField(rec, id_right), env->GetIntField(rec, id_bottom));
}

vector3df createvector3dfFromVector3d(JNIEnv *env, jobject vec)
{
	return vector3df(env->GetDoubleField(vec, id_vx), env->GetDoubleField(vec, id_vy),
			env->GetDoubleField(vec, id_vz));
}

void initJNIInfo(JNIEnv *env, jobject vector, jobject color4, jobject color3, jobject rect)
{
	cls_vector3d = env->GetObjectClass(vector);
	cls_color4i = env->GetObjectClass(color4);
	cls_color3i = env->GetObjectClass(color3);
	cls_rect4i = env->GetObjectClass(rect);
	
	id_vx = env->GetFieldID(cls_vector3d, "x", "D");
	id_vy = env->GetFieldID(cls_vector3d, "y", "D");
	id_vz = env->GetFieldID(cls_vector3d, "z", "D");
	
	id_red = env->GetFieldID(cls_color4i, "red", "I");
	id_green = env->GetFieldID(cls_color4i, "green", "I");
	id_blue = env->GetFieldID(cls_color4i, "blue", "I");
	id_alpha = env->GetFieldID(cls_color4i, "alpha", "I");
	
	id_red3 = env->GetFieldID(cls_color3i, "red", "I");
	id_green3 = env->GetFieldID(cls_color3i, "green", "I");
	id_blue3 = env->GetFieldID(cls_color3i, "blue", "I");
	
	id_left = env->GetFieldID(cls_rect4i, "left", "I");
	id_top = env->GetFieldID(cls_rect4i, "top", "I");
	id_right = env->GetFieldID(cls_rect4i, "right", "I");
	id_bottom = env->GetFieldID(cls_rect4i, "bottom", "I");
}

long _getTime()
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}

IImage* createImageFromBitmap(JNIEnv* env, jobject jbitmap)
{
	if (device && !driver)
		driver = device->getVideoDriver();
		
	if (!driver)
	{
		LOGE("Driver not set!");
		return 0;
	}

	AndroidBitmapInfo bitmapInfo;
	int result = 0;
	if ((result = AndroidBitmap_getInfo(env, jbitmap, &bitmapInfo)) < 0)
	{
		LOGE("AndroidBitmap_getInfo() failed! error = %d", result);
		return 0;
	}
	
	ECOLOR_FORMAT format = ECF_UNKNOWN;
	switch (bitmapInfo.format)
	{
	case ANDROID_BITMAP_FORMAT_RGB_565:
		format = ECF_R5G6B5;
		break;
	case ANDROID_BITMAP_FORMAT_RGBA_8888:
		format = ECF_A8R8G8B8;
		break;
	default: break;
	}
	
	if (format == ECF_UNKNOWN)
	{
		LOGE("Unsurpported color format!");
		return 0;
	}
	
	void *pixels = 0;
	
	if ((result = AndroidBitmap_lockPixels(env, jbitmap, &pixels)) < 0 || pixels == 0)
	{
		LOGE("AndroidBitmap_lockPixels() failed! error = %d", result);
		return 0;
	}
	
	IImage* image = driver->createImageFromData(format, 
		dimension2d<u32>(bitmapInfo.width, bitmapInfo.height), pixels);
		
	return image;
}

//not safe, will cause fatal error when it is not a opengles texture.
int getOpenGLESTextureID(const ITexture* tex)
{
	return ((COGLES1Texture*)tex)->getOGLES1TextureName();
}