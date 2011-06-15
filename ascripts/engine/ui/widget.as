
package engine.ui;

use engine.shapes.rectangle;

use arraylist;

abstract class Widget
{
	private string				mName;
	private Widget            	mParent;
	private Rectangle		  	mBounds;
	private Border				mBorder;
	
	public Widget()
	{
		super();
	}
	
	// - Get/set
	
	public Widget getParent()
	{
		return mParent;
	}
	
	public void setParent(Widget parent)
	{
		mParent = parent;
	}
	
	// - painting
	
	public void paint(Graphics graphics)
	{
		paintBackground(graphics);
		
		graphics.translate(getBounds().x, getBounds().y);
		
		paintBorder(graphics);
		paintWidget(graphics);
		
		graphics.translate(-getBounds().x, -getBounds().y);
	}
	
	abstract protected void paintBackground(Graphics graphics);
	abstract protected void paintWidget(Graphics graphics);
	
	protected void paintBorder(Graphics graphics)
	{
		if ( mBorder != null )
		{
			mBorder.paint(graphics);
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
	
	// - border
	
	public void setBorder(Border border)
	{
		mBorder = border;
	}
	
}
