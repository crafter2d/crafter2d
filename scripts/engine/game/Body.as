
package engine.game;

use engine.core.*;

abstract class Body
{
	public abstract void addForceGenerator(ForceGenerator generator);
	
	public abstract real getMass();
	public abstract Vector2D getLinearVelocity();
}
