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
video::SColor backColor = video::SColor(100,100,100,100);

extern "C"
{
	void Java_zte_test_irrlib_IrrCanvas_nativeSetClearColor(
		JNIEnv *env, jobject defaultObj,jint r, jint g, jint b, jint a )
	{
		backColor = video::SColor(a,r,g,b);
		//driver->beginScene(true, true, video::SColor(a,r,g,b));
		//driver->endScene();
	}
	
	
	void Java_zte_test_irrlib_IrrCanvas_nativeDrawImage(
		JNIEnv *env, jobject defaultObj, jstring path, jint left, jint up, jint width, jint height, jint transparent)
	{
		const char *msg = env->GetStringUTFChars(path,0);
		core::stringc imagePath = msg;
		video::ITexture* images = driver->getTexture(imagePath.c_str());
		core::dimension2d<u32> size = images->getOriginalSize();
		video::SColor *color = new video::SColor(transparent,255,255,255);


		driver->enableMaterial2D();
		driver->draw2DImage(images, core::rect<s32>(left,up,left+width,up+height),
				core::rect<s32>(0,0,size.Width,size.Height));
				//, 0,color, true);

		driver->enableMaterial2D(false);
	}
	
	void Java_zte_test_irrlib_IrrCanvas_nativeDrawRectangle(
		JNIEnv *env, jobject defaultObj,jint left, jint up, jint width, jint height,  jint r, jint g, jint b, jint transparent)
	{
		driver->draw2DRectangle(video::SColor(transparent,r,g,b),
				core::rect<s32>(left, up, left+width, up+height));
	}
	
	
	
	void Java_zte_test_irrlib_IrrCanvas_nativeDrawText(
		JNIEnv *env, jobject defaultObj,jstring text, jint left, jint up, jfloat size, jint r, jint g, jint b, jint a)
	{
		gui::IGUIFont* font = device->getGUIEnvironment()->getFont("/data/data/com.ellismarkov.irrlicht/files/Irrlicht/fonthaettenschweiler.bmp");
		if(!font) font = device->getGUIEnvironment()->getBuiltInFont();
		if (font){
			const char *msg = env->GetStringUTFChars(text,0);
			core::stringc str = msg;
			font->draw(str,core::rect<s32>(left,up,left+size*100,up+size),video::SColor(a,r,g,b));
		}
	}
	
}

