package zte.test.irrlib;

public class Color3i {
	private int red, green, blue;
	
	public Color3i(){
		red = green = blue = 0;
	}
	
	public Color3i(int v1, int v2, int v3){
		red = v1; green = v2; blue = v3;
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
}
