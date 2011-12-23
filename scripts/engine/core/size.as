
package engine.core;

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
	
	public void extend(int horiz, int vert)
	{
		width = width + horiz;
		height = height + vert;
	}
}
