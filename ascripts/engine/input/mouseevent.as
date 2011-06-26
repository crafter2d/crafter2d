
package engine.input;

class MouseEvent
{
	static public int LEFT_BUTTON = 1;
	static public int RIGHT_BUTTON = 2;
	static public int MIDDLE_BUTTON = 4;
	
	static public int ePressed = 0;
	static public int eReleased = 1;
	static public int eMotion = 2;

	private int 	mButton;
	private Point	mLocation;
	private int	 	mEvent;
	
	public MouseEvent(int button, Point location, int event)
	{
		super();
		mButton = button;
		mLocation = location;
		mEvent = event;
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
		return mEvent == ePressed;
	}
}
