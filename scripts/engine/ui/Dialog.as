
package engine.ui;

use engine.core.Color;

class Dialog extends Container
{
	private string	mTitle;
	
	public Dialog()
	{
		this("<no title>");
	}
	
	public Dialog(string title)
	{
		super();
		
		mTitle = title;
		setFont(FontManager.getInstance().getFont("amersn.ttf", 9));
		
		setBorder(new DialogBorder(this));
	}
	
	protected void paintBackground(Graphics graphics)
	{
		graphics.setColor(Color.buttonface);
		graphics.fillRect(getBounds());
	}
	
	protected void paintWidget(Graphics graphics)
	{
		graphics.setFont(getFont());
		graphics.setColor(Color.white);
		graphics.drawText(10, 11, mTitle);
		
		super.paintWidget(graphics);
	}
}
