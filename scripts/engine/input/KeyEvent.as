
package engine.input;

class KeyEvent
{
	private int 	mKey;
	private boolean mPressed;
	
	public KeyEvent(int key, boolean pressed)
	{
		super();
		
		mKey = key;
		mPressed = pressed;
	}
	
	// - get/set
	
	public int getKey()
	{
		return mKey;
	}
	
	public boolean isPressed()
	{
		return mPressed;
	}
}
