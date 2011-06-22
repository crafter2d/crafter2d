
package engine.input;

use engine.ui.*;

class MouseEventDispatcher extends InputEventDispatcher
{
	public native MouseEventDispatcher();
	
	// called by the VM
	protected void onMouseEvent(int button, int x, int y, boolean pressed)
	{
		// build event
		Point location = new Point(x, y);
		MouseEvent event = new MouseEvent(button, location, pressed);
		
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
