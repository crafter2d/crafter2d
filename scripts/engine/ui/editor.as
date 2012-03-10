
package engine.ui;

use engine.core.*;

class Editor extends Widget
{
	private string mText;
	
	public Editor()
	{
		super();
		
		setBorder(new SimpleBorder());
	}
	
	// - painting
	
	protected void paintBackground(Graphics graphics)
	{
		graphics.setColor(Color.white);
		graphics.fillRect(getBounds());
	}
	
	protected void paintWidget(Graphics graphics)
	{
		graphics.setColor(Color.black);
		graphics.drawText(mX, mY, mText);
	}
	
	// - Sizes
	
	public Size getPreferredSize()
	{
		return new Size();
	}
}
