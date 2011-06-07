
package engine.ui;

use engine.shapes.rectangle;

use arraylist;

abstract class Widget
{
	private Widget            mParent;
	private ArrayList<Widget> mChildren;
	private Rectangle		  mBounds;
	
	public Widget()
	{
		mParent = null;
		mChildren = new ArrayList<Widget>();
		mBounds = null;
	}
	
	// - children
	
	public void setParent(Widget parent)
	{
		mParent = parent;
	}
	
	public void add(Widget child)
	{
		child.setParent(this);
		
		mChildren.add(child);
	}
	
	// - painting
	
	public void paint(Graphics graphics)
	{
		paintWidget(graphics);
		paintChildren(graphics);
	}
	
	abstract protected void paintWidget(Graphics graphics);
	
	protected void paintChildren(Graphics graphics)
	{
		foreach( Widget child : mChildren )
		{
			child.paint(graphics);
		}
	}
	
	// - size & movement
	
	public Rectangle getBounds()
	{
		return mBounds;
	}
	
	public void setBounds(Rectangle bounds)
	{
		mBounds = bounds;
	}
}
