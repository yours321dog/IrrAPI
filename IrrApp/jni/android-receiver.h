#ifndef __ANDROID_RECEIVER_H_INCLUDED__
#define __ANDROID_RECEIVER_H_INCLUDED__

#include <irrlicht.h>

using namespace irr;

using namespace gui;

enum
{
        GUI_ID_QUIT_BUTTON = 101,
        GUI_ID_NEW_WINDOW_BUTTON,
        GUI_ID_FILE_OPEN_BUTTON,
        GUI_ID_TRANSPARENCY_SCROLL_BAR
};


struct SAppContext
{
    IrrlichtDevice *device;
    s32          counter;
    IGUIListBox*    listbox;
};


class AndroidEventReceiver : public IEventReceiver
{
private:
    SAppContext & Context;
	// We use this array to store the current state of each key
	bool KeyIsDown;     //按键当前的状态
	// We use this array to store whether the current key is pressed and not handled
	bool KeyIsPressed;  //是否有点击事件发生
	f32 x, y, ox, oy;
	u16 direction;  //记录手势识别所获得的的方向
public:

    //constructor
    AndroidEventReceiver(SAppContext & context) : Context(context) {
        KeyIsDown = false;
        KeyIsPressed = false;
        x = 0;
        y = 0;
		ox = 0;
		oy = 0;
		direction = 0;
    }
    virtual bool OnEvent(const SEvent& event);
	bool IsKeyDown() const;
	bool IsKeyPressed();
	f32 getX();
	f32 getY();
	u16 getDI();
	void setDI(u16 ndi);
};

#endif // __ANDROID_RECEIVER_H_INCLUDED__
