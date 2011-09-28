
package engine.ui;

class Size
{
	int width;
	int height;
	
	public Size()
	{
		this(0, 0);
	}
	
	public Size(int w, int h)
	{
		super();
		width = w;
		height = h;
	}
	
	public void extend(Margins margins)
	{
		width = width + margins.left + margins.right;
		height = height + margins.top + margins.bottom;
	}
	
	public void extend(int horiz, int vert)
	{
		width = width + horiz;
		height = height + vert;
	}
}
