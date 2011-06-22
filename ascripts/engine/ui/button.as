
package engine.ui;

use engine.shapes.*;

class Button extends Widget
{
	private string mText;
	private int    mX;
	private int    mY;
	
	public Button(string text)
	{
		super();
		
		mText = text;
		
		setFont(FontManager.getInstance().getFont("c:\\windows\\fonts\\segoeui.ttf", 9)); //"amersn.ttf", 10));
		setBorder(new SimpleBorder());
	}
	
	// - notifications
	
	protected void boundsChanged()
	{
		Font f = getFont();
		Rectangle bounds = getBounds();
		Rectangle r = f.getTextSize(mText);
		mX = (bounds.width / 2) - (r.width / 2);
		mY = (bounds.height / 2) - (r.height / 2) + f.getBaseLine();
	}
	
	// - painting
	
	protected void paintBackground(Graphics graphics)
	{
		graphics.setColor(Color.buttonface);
		graphics.fillRect(getBounds());
	}
	
	protected void paintWidget(Graphics graphics)
	{
		graphics.setColor(Color.black);
		graphics.drawText(mX, mY, mText);
	}
}
