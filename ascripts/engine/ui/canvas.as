
package engine.ui;

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
	
	// maintenance
	
	public void add(Widget widget)
	{
		mWidgets.add(widget);
	}
	
	public void remove(Widget widget)
	{
		mWidgets.remove(widget);
	}
	
	// painting
	
	public void paint(Graphics graphics)
	{
		foreach( Widget w : mWidgets )
		{
			w.paint(graphics);
		}
	}
}
