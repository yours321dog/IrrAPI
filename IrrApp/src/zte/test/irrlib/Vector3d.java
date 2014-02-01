package zte.test.irrlib;

public class Vector3d {
	 
	public Vector3d(){
		
	}	
	
	public Vector3d(Vector3d v){
		this.x = v.x;
		this.y = v.y;
		this.z = v.z;
	}
	
	public Vector3d(double x, double y, double z){
		this.x = x; this.y = y; this.z = z;
	}
	
	public Vector3d plus(Vector3d v){
		return new Vector3d(x+v.x, y+v.y, z+v.z);
	}
	
	public Vector3d minus(Vector3d v){
		return new Vector3d(x-v.x, y-v.y, z-v.z);
	}
	
	public Vector3d multi(Vector3d v){
		return new Vector3d(x*v.x, y*v.y, z*v.z);
	}
	
	public double distanceSquare(Vector3d v){
		return (v.x-x)*(v.x-x) + (v.y-y)*(v.y-y) + (v.z-z)*(v.z-z);
	}
	
	public void copy(Vector3d v){
		x = v.x; y = v.y; z = v.z;
	}
	
	public double x, y, z;
}
