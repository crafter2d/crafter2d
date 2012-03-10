
package engine.ui;

use engine.core.*;
use engine.shapes.*;

class Label extends Widget
{
	private string mText;
	private int    mY;
	
	public Label()
	{
		super();
	}
	
	// - get/set
	
	public string getText()
	{
		return mText;
	}
	
	public void setText(string text)
	{
		mText = text;
	}
	
	// - notifications
	
	protected void boundsChanged()
	{
		Font f = getFont();
		Rectangle bounds = getBounds();
		Size textsize = f.getTextSize(mText);
		mY = (bounds.height / 2) - (textsize.height / 2) + f.getBaseLine();
	}
	
	// - drawing
	
	protected void paintBackground(Graphics graphics)
	{
	}
	
	protected void paintWidget(Graphics graphics)
	{
		if ( mText != null )
		{
			Font f = getFont();
			
			graphics.setColor(Color.black);
			graphics.drawText(0, mY, mText);
		}
	}
}
