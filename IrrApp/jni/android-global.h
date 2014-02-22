
#ifndef __ANDOIRD_GLOBAL_H_INCLUDED__
#define __ANDOIRD_GLOBAL_H_INCLUDED__

#include <irrlicht.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <jni.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

long _getTime();
int getOpenGLESTextureID(const ITexture* tex);

void initJNIInfo(JNIEnv *env, jobject vector, jobject color4, jobject color3, jobject rect);
IImage* createImageFromBitmap(JNIEnv* env, jobject jbitmap);

recti createrectiFromRect4i(JNIEnv *env, jobject rec);
SColor createSColorFromColor4i(JNIEnv *env, jobject color);
SColor createSColorFromColor3i(JNIEnv *env, jobject color);
SColorf createSColorfFromColor3i(JNIEnv *env, jobject color);
vector3df createvector3dfFromVector3d(JNIEnv *env, jobject vec);

#define LOG_TAG "irrlicht engine"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#define WARN_PARENT_NOT_FOUND(id, funcName) \
	LOGW("Parent Node(id: %d) not found! In Function: %s", id, #funcName)

#define WARN_NODE_NOT_FOUND(id, funcName) \
	LOGW("Node(id: %d) not found! In Function: %s", id, #funcName)
	
#define ERROR_ADD_FAILD(id, funcName) \
	LOGE("Add Node(id: %d) failed! In Function: %s", id, #funcName)

#define INFO_ADD_SUCCEED(id, funcName) \
	LOGI("Add Node(id: %d) success! In Function: %s", id, #funcName)
	
extern IrrlichtDevice *device;
extern IVideoDriver* driver;
extern ISceneManager* smgr;

extern int  gWindowWidth;
extern int  gWindowHeight;
extern stringc gSdCardPath;
extern video::SColor backColor;

extern bool _isInit;
extern char _extPrefix[];
extern char _builtInFontPath[];
extern ITexture* _extTex;

#endif // __ANDOIRD_GLOBAL_H_INCLUDED__
