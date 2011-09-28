
package engine.ui;

class Margins
{
	public int left;
	public int top;
	public int right;
	public int bottom;
	
	public Margins()
	{
		this(0, 0, 0, 0);
	}
	
	public Margins(int left, int top, int right, int bottom)
	{
		super();
		set(left, top, right, bottom);
	}
	
	// - Operations
	
	public void set(int size)
	{
		set(size, size, size, size);
	}
	
	public void set(int left, int top, int right, int bottom)
	{
		this.left = left;
		this.top = top;
		this.right = right;
		this.bottom = bottom;
	}
}
