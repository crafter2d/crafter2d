
package engine.ui;

use engine.collections.ArrayList;
use engine.core.*;
use engine.shapes.*;

abstract class Container extends Widget
{
	private ArrayList<Widget> 	mChildren;
	private LayoutManager		mLayout;
	
	public Container()
	{
		super();
		
		mChildren = new ArrayList<Widget>();
	}
	
	// - Get/set
	
	public LayoutManager getLayout()
	{
		return mLayout;
	}
	
	public void setLayout(LayoutManager layout)
	{
		mLayout = layout;
	}
	
	// - Operations
	
	public void validate()
	{
		if ( mLayout != null )
		{
			mLayout.layout(this);
		}
	}
	
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
	
	// - Children
	
	public void add(Widget child, Object constraints)
	{
		child.setParent(this);
		
		mChildren.add(child);
		
		if ( mLayout != null )
		{
			mLayout.addWidget(child, constraints);
		}
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
