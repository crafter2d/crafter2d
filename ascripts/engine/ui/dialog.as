
package engine.ui;

class Dialog extends Container
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
		
		setBorder(new DialogBorder(this));
	}
	
	protected void paintBackground(Graphics graphics)
	{
		graphics.setColor(Color.white);
		graphics.fillRect(getBounds());
	}
	
	protected void paintWidget(Graphics graphics)
	{
		graphics.setFont(mFont);
		graphics.setColor(Color.white);
		graphics.drawText(10, 11, mTitle);
		
		super.paintWidget(graphics);
	}
}
