
package engine.ui;

abstract class Container extends Widget
{
	private ArrayList<Widget> 	mChildren;
	
	public Container()
	{
		super();
		
		mChildren = new ArrayList<Widget>();
	}
	
	// - children
	
	public void add(Widget child)
	{
		child.setParent(this);
		
		mChildren.add(child);
	}
	
	// - Painting
	
	protected void paintWidget(Graphics graphics)
	{
		paintChildren(graphics);
	}
	
	protected void paintChildren(Graphics graphics)
	{	
		foreach( Widget child : mChildren )
		{
			child.paint(graphics);
		}
	}
}
