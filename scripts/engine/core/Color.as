
package engine.core;

class Color
{
	private real mRed;
	private real mGreen;
	private real mBlue;
	private real mAlpha;
	
	static public Color buttonface  = new Color(230, 230, 230, 255);
	static public Color white 		= new Color(255, 255, 255, 255);
	static public Color gray  	    = new Color(192, 192, 192, 255);
	static public Color lightgray   = new Color(226, 226, 226, 255);
	static public Color darkgray    = new Color(116, 116, 116, 255);
	static public Color red   		= new Color(255, 0, 0, 255);
	static public Color black 		= new Color(0, 0, 0, 255);
	
	public Color()
	{
		this(1.0, 1.0, 1.0, 0.0);
	}
	
	public Color(int r, int g, int b, int a)
	{
		this(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
	}
	
	public Color(real r, real g, real b, real a)
	{
		super();
		
		mRed = r;
		mGreen = g;
		mBlue = b;
		mAlpha = a;
	}
	
	public real getRed()
	{
		return mRed;
	}
	
	public real getGreen()
	{
		return mGreen;
	}
	
	public real getBlue()
	{
		return mBlue;
	}
	
	public real getAlpha()
	{
		return mAlpha;
	}
}
