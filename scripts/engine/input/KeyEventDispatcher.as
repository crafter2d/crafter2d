
package engine.input;

use engine.ui.InputFocusManager;

class KeyEventDispatcher extends InputEventDispatcher
{
	public KeyEventDispatcher()
	{
		super();
	}
	
	// called by the VM
	public void dispatch(int key, boolean pressed)
	{
		KeyEvent event = new KeyEvent(key, pressed);
		
		Widget widget = (Widget) InputFocusManager.getInstance().getFocus();
		widget.fireKeyEvent(event);
	}
}
