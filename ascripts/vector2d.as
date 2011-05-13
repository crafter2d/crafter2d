
class Vector2D
{
	public native Vector2D();
	public native real getX();
	public native void setX(real x);
	public native real getY();
	public native void setY(real y);
	
	public void offsetX(real amount)
	{
		setX(getX() + amount);
	}
	
	public void set(real x, real y)
	{
		setX(x);
		setY(y);
	}
}
