
package engine.game;

abstract class Entity
{
	public native int getId();
	
	public void onCreated()
	{
	}
	
	// - Get/set
	
	public Vector2D getPosition()
	{
		return new Vector2D(getPositionX(), getPositionY());
	}
	
	public void setPosition(Vector2D pos)
	{
		setPosition(pos.x, pos.y);
	}
	
	public void setController(Controller c)
	{
		native(c);
		mController = c;
	}

	// - Natives
	
	private native real getPositionX();
	private native real getPositionY();
	private native void setPosition(real x, real y);
	
	public native void setName(string name);
	public native boolean hasLineOfSight(Actor to);
	public native boolean direction();
	public native void flip();
		
	protected native void sendComponentMessage(ComponentMessage message);
}
