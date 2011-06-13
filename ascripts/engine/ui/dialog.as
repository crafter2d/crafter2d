
package engine.ui;

class Dialog extends Widget
{
	private string	mTitle;
	private Font    mFont;
	
	public Dialog()
	{
		this("<no title>");
	}
	
	public Dialog(string title)
	{
		super();
		mTitle = title;
		mFont = FontManager.getInstance().getFont("amersn.ttf", 10);
	}
	
	protected void paintBackground(Graphics graphics)
	{
		Color color = new Color(255, 0, 0, 255);
		graphics.setColor(color);
		graphics.fillRect(getBounds());
	}
	
	protected void paintWidget(Graphics graphics)
	{
		graphics.setFont(mFont);
		graphics.setColor(new Color(255,255,255,0));
		graphics.drawText(10, 10, mTitle);
	}
}
