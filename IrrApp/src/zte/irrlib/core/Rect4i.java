package zte.irrlib.core;

public class Rect4i {
	
	public Rect4i(){
		left = top = right = bottom = 0;
	}
	
	public Rect4i(int left, int up, int right, int bottom){
		this.left = left;
		this.right = right;
		this.top = up;
		this.bottom = bottom;
	}
	
	public int left, right, top, bottom;
}
