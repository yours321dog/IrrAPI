package zte.irrlib;

import android.view.KeyEvent;
import android.view.MotionEvent;

public class Event implements Runnable{
	
	public static final int NULL_EVENT = 0x00;

	public static final int TOUCH_EVENT = 0x01;
	public static final int KEY_DOWN_EVENT = 0x02;
	
	public int EventType = NULL_EVENT;
	
	//key event;
	public int keyCode;
	public KeyEvent keyEvent;
	
	//touch event;
	public MotionEvent motionEvent;

	public Event(Engine engine){
		mEngine = engine;
	}
	
	public void run() {
		mEngine.setEvent(this);
	}
	
	private Engine mEngine;
}
