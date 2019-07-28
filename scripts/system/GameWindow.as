
package system;

use engine.input.*;

class GameWindow
{
	private int mWidth;
	private int mHeight;
	
	public boolean create(string title, int width, int height, int bitdepth, boolean fullscreen)
	{
		mWidth = width;
		mHeight = height;
		
		return native(title, width, height, bitdepth, fullscreen);
	}
	
	public native void setUiSystem(UiSystem system);
	
	public int getWidth()
	{
		return mWidth;
	}
	
	public int getHeight()
	{
		return mHeight;
	}
}
