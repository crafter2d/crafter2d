
package engine.input

class MouseEvent
{
	static public int LEFT_BUTTON = 1;
	static public int RIGHT_BUTTON = 2;
	static public int MIDDLE_BUTTON = 4;

	private int 	mButton;
	private Point	mLocation;
	private boolean mDown;
	
	public MouseEvent(int button, Point location, boolean down)
	{
		mButton = button;
		mLocation = location;
		mDown = down;
	}
	
	// - Get/set
	
	public int getButton()
	{
		return mButton;
	}
	
	public boolean isLeftButton()
	{
		return (mButton & LEFT_BUTTON) == LEFT_BUTTON;
	}
	
	public Point getLocation()
	{
		return mLocation;
	}
	
	public boolean isDown()
	{
		return mDown;
	}
}
