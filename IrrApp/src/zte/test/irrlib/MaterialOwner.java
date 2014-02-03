package zte.test.irrlib;

public interface MaterialOwner {
	
	void setAmbientColor(Color4i color, int materialID);
	void setDiffuseColor(Color4i color, int materialID);
	void setEmissiveColor(Color4i color, int materialID);
	void setSpecularColor(Color4i color, int materialID);
	void setShininess(double para, int materialID);
	
	//return 0 if succeed
	int setTexture(String path, int materialID);
	int addTextureAnimator(String[] path, int timePerFrame, boolean loop);
	
	void setTouchable(boolean flag);
}
