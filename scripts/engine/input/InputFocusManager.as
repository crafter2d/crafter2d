
package engine.input;

class InputFocusManager
{
	static private InputFocusManager mInstance = new InputFocusManager();
	static public InputFocusManager getInstance()
	{
		return mInstance;
	}
	
	static public void setInputFocus(Focussable focus)
	{
		mInstance.setFocus(focus);
	}
	
	private Focussable mFocus;
	
	// - get/set
	
	public Focussable getFocus()
	{
		return mFocus;
	}
	
	public void setFocus(Focussable focus)
	{
		mFocus = focus;
	}
};
