
package engine.ui;

use engine.collections.*;
use engine.core.*;

class Canvas
{
	private static Canvas mInstance = new Canvas();
	
	public static Canvas getInstance()
	{
		if ( mInstance == null )
		{
			mInstance = new Canvas();
		}
		return mInstance;
	}
	
	private ArrayList<Widget> mWidgets = new ArrayList<Widget>();
	
	private Canvas()
	{
		super();
	}
	
	// - Maintenance
	
	public void add(Widget widget)
	{
		mWidgets.add(widget);
	}
	
	public void remove(Widget widget)
	{
		mWidgets.remove(widget);
	}
	
	// - Painting
	
	public void paint(Graphics graphics)
	{
		foreach( Widget w : mWidgets )
		{
			w.paint(graphics);
		}
	}
	
	// - Searching
	
	Widget findWidget(Point location)
	{
		foreach( Widget w : mWidgets )
		{
			Widget focus = w.hitTest(location);
			if ( focus != null )
			{
				return focus;
			}
		}
		return null;
	}
}