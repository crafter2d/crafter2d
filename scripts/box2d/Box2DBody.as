
package box2d;

use engine.core.*;
use engine.game.*;

class Box2DBody extends Body
{
	public native void addForceGenerator(ForceGenerator generator)
	{
		native;
	}
	
	public real getMass()
	{
		return native;
	}
	
	public Vector2D getLinearVelocity()
	{
		return native;
	}
}
