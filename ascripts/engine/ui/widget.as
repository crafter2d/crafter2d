
package engine.ui;

use engine.shapes.rectangle;
use engine.input.*;

use arraylist;

abstract class Widget
{
	private string						mName;
	private Font    					mFont;
	private Widget            			mParent;
	private Rectangle		  			mBounds;
	private Border						mBorder;
	private ArrayList<MouseListener> 	mMouseListeners;
	
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
	
	public Font getFont()
	{
		if ( mFont == null && mParent != null )
		{
			return mParent.getFont();
		}
		return mFont;
	}
	
	public void setFont(Font font)
	{
		mFont = font;
	}
	
	public boolean hitTest(Point point)
	{
		return false;
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
		boundsChanged();
	}
	
	protected void boundsChanged()
	{
		// placeholder for overloads
	}
	
	// - Border
	
	public void setBorder(Border border)
	{
		mBorder = border;
		mBorder.setWidget(this);
	}
	
	// - Listeners
	
	public void addMouseListener(MouseListener listener)
	{
		mMouseListeners.add(listener);
	}
}
