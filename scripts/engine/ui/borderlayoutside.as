
package engine.ui;

/**
 * BorderLayoutSide
 */
 
class BorderLayoutSide
{
	public static int LEFT   = 1;
	public static int RIGHT  = 2;
	public static int TOP    = 3;
	public static int BOTTOM = 4;
	public static int CENTER = 5;
	
	public static BorderLayoutSide left = new BorderLayoutSide(LEFT);
	public static BorderLayoutSide right = new BorderLayoutSide(RIGHT);
	public static BorderLayoutSide top = new BorderLayoutSide(TOP);
	public static BorderLayoutSide bottom = new BorderLayoutSide(BOTTOM);
	public static BorderLayoutSide center = new BorderLayoutSide(CENTER);
	
	private int mSide;
	
	public BorderLayoutSide(int side)
	{
		super();
		
		mSide = side;
	}
	
	public int getSide()
	{
		return mSide;
	}
}
