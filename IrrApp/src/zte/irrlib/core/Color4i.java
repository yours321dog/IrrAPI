package zte.irrlib.core;

public class Color4i {
	private int red, green, blue, alpha;
	
	//default white
	public Color4i(){ 
		red = green = blue = alpha = 0xff;
	}
	
	public Color4i(int v1, int v2, int v3, int v4){
		red = v1; green = v2; blue = v3; alpha = v4;
	}
	
	public int r() {
		if (red < 0) return 0;
		else if (red > 255) return 255;
		else return red;
	}
	
	public int g() {
		if (green < 0) return 0;
		else if (green > 255) return 255;
		else return green;
	}
	
	public int b() {
		if (blue < 0) return 0;
		else if (blue > 255) return 255;
		else return blue;
	}
	
	public int a() {
		if (alpha < 0) return 0;
		else if (alpha > 255) return 255;
		else return alpha;
	}
}
