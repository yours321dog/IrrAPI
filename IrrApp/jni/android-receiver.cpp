#include <android-receiver.h>
#include <android/log.h>


extern int gAppAlive;

bool AndroidEventReceiver::OnEvent(const SEvent& event)
{
    //__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "Lolo");

    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {                //有触屏事件发生
    	KeyIsPressed = true;
    	KeyIsDown = true;
    	x = event.MouseInput.X;     //记录触点x轴坐标
    	y = event.MouseInput.Y;     //记录触点y轴坐标
		if(event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {   //触屏事件为刚按下
			ox = x;                 //记录按下时的触点x轴坐标
			oy = y;                 //记录按下时的触点y轴坐标
		}
		else if(event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {   //触屏事件为手指离开屏幕
			if( (ox-x) > 30 ) direction = 1;        //离开位置x轴坐标明显小于按下时的x轴坐标
			else if( (x-ox) > 30 ) direction = 2;   //离开位置x轴坐标明显大于按下时的x轴坐标
			else direction = 0;     //坐标差距不大，视为无手势滑动
		}
    }


    if (event.EventType == EET_GUI_EVENT)   //有点击按钮事件发生
    {
        s32 id = event.GUIEvent.Caller->getID();
        IGUIEnvironment* env = Context.device->getGUIEnvironment();

        //__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "Lolo 2: %d %d %d %d", event.GUIEvent.EventType, EGET_BUTTON_CLICKED, id , GUI_ID_QUIT_BUTTON);
        switch(event.GUIEvent.EventType)
        {
            case EGET_BUTTON_CLICKED:   //按下退出键
               // __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "Lolo 3");
                switch(id)
                {
                    case GUI_ID_QUIT_BUTTON:
                       // __android_log_print(ANDROID_LOG_INFO, "Irrlicht", "Lolo 4");
                        gAppAlive = 0;  //杀死应用
                        return true;

                    default:
                        break;
                }
        }
    }
    return false;
}
bool AndroidEventReceiver::IsKeyDown() const
{
	return KeyIsDown;
}

bool AndroidEventReceiver::IsKeyPressed()
{
	if (KeyIsPressed) {
		KeyIsPressed = false;
		return true;
	}
	return false;
}

f32 AndroidEventReceiver::getX() {
	return x;
}
f32 AndroidEventReceiver::getY() {
	return y;
}
u16 AndroidEventReceiver::getDI() {
	return direction;
}

//用于重置方向值
void AndroidEventReceiver::setDI(u16 ndi){
	direction = ndi;
}
