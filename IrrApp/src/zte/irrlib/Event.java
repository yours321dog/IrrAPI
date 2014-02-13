package zte.irrlib;

import android.view.KeyEvent;
import android.view.MotionEvent;

public class Event {
	
	public static final int NULL_EVENT = 0x00;

	public static final int TOUCH_EVENT = 0x01;
	public static final int KEY_DOWN_EVENT = 0x02;
	
	public int EventType = NULL_EVENT;
	
	//key event;
	public int KeyCode;
	public KeyEvent KeyEvent;
	
	//touch event;
	public MotionEvent MotionEvent;
	
	//to be continue;
}
