
package engine.ui;

abstract class Border
{
	private Widget  mWidget;
	private Margins mMargins;
	
	public Border()
	{
		super();
		
		mMargins = new Margins();
	}
	
	// - get/set
	
	public Widget getWidget()
	{
		return mWidget;
	}
	
	public void setWidget(Widget widget)
	{
		mWidget = widget;
	}
	
	public Margins getMargins()
	{
		return mMargins;
	}
	
	void setMargins(Margins margins)
	{
		mMargins = margins;
	}
	
	// - Painting
	
	abstract public void paint(Graphics graphics);
}
