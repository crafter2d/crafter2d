
package engine.shapes;

class Rectangle
{
	public int x;
	public int y;
	public int width;
	public int height;
	
	public Rectangle()
	{
		this(0, 0, 0, 0);
	}
	
	public Rectangle(int ix, int iy, int w, int h)
	{
		super();
		
		set(ix, iy, w, h);
	}
	
	public void set(int ix, int iy, int w, int h)
	{
		x = ix;
		y = iy;
		width = w;
		height = h;
	}
}
