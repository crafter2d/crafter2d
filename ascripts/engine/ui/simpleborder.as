
package engine.ui;

use engine.shapes.*;

class SimpleBorder extends Border
{
	private Rectangle mBorderRect = new Rectangle();
	
	public void paint(Graphics graphics)
	{
		Rectangle rect = getWidget().getBounds();

		mBorderRect.set(0, 0, rect.width, rect.height);
		graphics.setColor(Color.darkgray);
		graphics.drawRoundedRect(mBorderRect);
		
		mBorderRect.set(1, 1, rect.width - 2, rect.height - 2);
		graphics.setColor(Color.white);
		graphics.drawRoundedRect(mBorderRect);
	}
}
