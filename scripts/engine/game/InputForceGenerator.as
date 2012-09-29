
package engine.game;

use engine.core.*;

class InputForceGenerator
{
	public native InputForceGenerator();
	
	public void setVelocity(Vector2D vel)
	{
		native_setVelocity(vel.x, vel.y);
	}
	
	public void setImpulse(Vector2D impulse)
	{
		native_setImpulse(impulse.x, impulse.y);
	}
	
	private native void native_setVelocity(real x, real y);
	private native void native_setImpulse(real x, real y);
}
