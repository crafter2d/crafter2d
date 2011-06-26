
package engine.ui;

use engine.shapes.*;

abstract class Container extends Widget
{
	private ArrayList<Widget> 	mChildren;
	
	public Container()
	{
		super();
		
		mChildren = new ArrayList<Widget>();
	}
	
	// - Operations
	
	public Widget hitTest(Point location)
	{
		Rectangle bounds = getBounds();
		
		if ( bounds.contains(location) )
		{
			Point point = new Point();
			point.x = location.x - bounds.x;
			point.y = location.y - bounds.y;
			
			foreach ( Widget child : mChildren )
			{
				Widget result = child.hitTest(point);
				if ( result != null )
				{
					return result;
				}
			}
			
			return this;
		}
		return null;
	}
	
	// - children
	
	public void add(Widget child)
	{
		child.setParent(this);
		
		mChildren.add(child);
	}
	
	// - Painting
	
	protected void paintWidget(Graphics graphics)
	{
		paintChildren(graphics);
	}
	
	protected void paintChildren(Graphics graphics)
	{	
		foreach( Widget child : mChildren )
		{
			child.paint(graphics);
		}
	}
}
