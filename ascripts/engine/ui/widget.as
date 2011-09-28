
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
	
	public void setName(string name)
	{
		mName = name;
	}
	
	public Widget hitTest(Point point)
	{
		if ( mBounds.contains(point) )
		{
			return this;
		}
		return null;
	}
	
	public Size getPreferredSize()
	{
		return new Size(10, 10);
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
	
	public void setBounds(int left, int top, int width, int height)
	{
		setBounds(new Rectangle(left, top, width, height));
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
	
	public Border getBorder()
	{
		return mBorder;
	}
	
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
	
	public void fireMouseEvent(MouseEvent event)
	{
		if ( event.isLeftButton() && event.isDown() )
		{
			System.console.println("Left click on " + mName);
		}
	}
}
