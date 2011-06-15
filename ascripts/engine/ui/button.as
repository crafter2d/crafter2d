
package engine.ui;

class Button extends Widget
{
	private string mText;
	
	public Button(string text)
	{
		super();
		
		mText = text;
	}
	
	protected void paintBackground(Graphics graphics)
	{
		graphics.setColor(Color.lightgray);
		graphics.fillRect(getBounds());
	}
	
	protected void paintWidget(Graphics graphics)
	{
		Container parent = (Container)getParent();
	}
}
