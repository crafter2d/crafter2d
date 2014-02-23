
package engine.input;

use engine.core.*;

class MouseEventDispatcher extends InputEventDispatcher
{
	public MouseEventDispatcher()
	{
		super();
	}
	
	// called by the VM
	public void dispatch(int x, int y, int button, int event)
	{
		// build event
		Point location = new Point(x, y);
		MouseEvent event = new MouseEvent(button, location, event);
	}
}
