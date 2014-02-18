#include <sys/time.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device = 0;
IVideoDriver* driver = 0;
ISceneManager* smgr = 0;
ISceneCollisionManager *collMgr = 0;

int gWindowWidth = 640;
int gWindowHeight = 480;
stringc gSdCardPath = "/sdcard/";
SColor backColor = SColor(255,180,180,255);

bool _isInit = false;

long _getTime()
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}

Image* createImageFromBitmap(JNIEnv* env, jobject jbitmap)
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
		LOGE("Unsurpported color format!")
		return 0;
	}
	
	void *pixels = 0;
	
	if ((result = AndroidBitmap_lockPixels(env, jbitmap, pixels))) < 0 || pixels == 0)
	{
		LOGE("AndroidBitmap_lockPixels() failed! error = %d", result);
		return 0;
	}
	
	Image* image = driver->createImageFromData(format, 
		dimension2d<u32>(bitmapInfo.width, bitmapInfo.height), pixels);
		
	return image;
}