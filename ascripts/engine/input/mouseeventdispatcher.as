
package engine.input;

use engine.ui.*;

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
		
		// search for the widget
		Canvas canvas = Canvas.getInstance();
		Widget widget = canvas.findWidget(location);
		if ( widget != null )
		{
			// dispatch the event
			widget.fireMouseEvent(event);
		}
	}
}
