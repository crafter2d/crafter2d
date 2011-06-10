
package engine.ui;

class Dialog extends Widget
{
	private string	mTitle;
	
	public Dialog()
	{
		super();
	}
	
	public Dialog(string title)
	{
		super();
		mTitle = title;
	}
	
	protected void paintBackground(Graphics graphics)
	{
		Color color = new Color(255, 0, 0, 255);
		graphics.setColor(color);
		graphics.fillRect(getBounds());
	}
	
	protected void paintWidget(Graphics graphics)
	{
		graphics.drawText(10, 10, mTitle);
	}
}
