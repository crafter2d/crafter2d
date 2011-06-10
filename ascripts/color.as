
class Color
{
	private real mRed;
	private real mGreen;
	private real mBlue;
	private real mAlpha;
	
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
