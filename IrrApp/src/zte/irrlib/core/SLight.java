package zte.irrlib.core;


public class SLight {
	
	public static final int POINT_LIGHT = 0x01;
	public static final int SPOT_LIGHT = 0x02;
	public static final int DIRECTIONAL_LIGHT = 0x03;
	
	public int Type = POINT_LIGHT;
	
	public Color3i AmbientColor = new Color3i();
	public Color3i DiffuseColor = new Color3i(0xff, 0xff, 0xff);
	public Color3i SpecularColor = new Color3i(0xff, 0xff, 0xff);
	
	//Attenuation factors (constant, linear, quadratic)
	/* Changes the light strength fading over distance.
	Can also be altered by setting the radius, Attenuation will change to
	(0,1.f/radius,0). Can be overridden after radius was set. */
	public Vector3d Attenuation = new Vector3d(0, 0, 1.0);
	
	//only available for spot light
	public double OuterCone = 45.0;
	public double InnerCone = 0.0;
	public double Falloff = 2.0;
	
	public Vector3d Direction = new Vector3d(0, 0, 1.0);
	public double Radius = 100.0;
}
