
package engine.core;

final class Vector2D
{
	public real x = 0.0;
	public real y = 0.0;
	
	public Vector2D()
	{
		super();
	}
	
	public Vector2D(real x, real y)
	{
		super();
		
		this.x = x;
		this.y = y;
	}
	
	public real getX()
	{
		return x;
	}
	
	public void setX(real x)
	{
		this.x = x;
	}
	
	public real getY()
	{
		return y;
	}
	
	public void setY(real y)
	{
		this.y = y;
	}
	
	public real distance(Vector2D to)
	{
		real nx = to.x - x;
		real ny = to.y - y;
		
		return Math.sqrt((nx * nx) + (ny * ny));
	}
	
	public void offsetX(real amount)
	{
		x = x + amount;
	}
	
	public void set(real x, real y)
	{
		this.x = x;
		this.y = y;
	}
}
