
package engine.ui;

abstract class Border
{
	private Widget mWidget;
	
	// - get/set
	
	public Widget getWidget()
	{
		return mWidget;
	}
	
	public void setWidget(Widget widget)
	{
		mWidget = widget;
	}
	
	abstract public void paint(Graphics graphics);
}
