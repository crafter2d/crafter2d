
package engine.game;

use engine.core.*;

final class InputForceGenerator
{
	public native InputForceGenerator();
	
	public void setVelocity(Vector2D vel)
	{
		setVelocity(vel.x, vel.y);
	}
	
	public void setImpulse(Vector2D impulse)
	{
		setImpulse(impulse.x, impulse.y);
	}
	
	private native void setVelocity(real x, real y);
	private native void setImpulse(real x, real y);
}
