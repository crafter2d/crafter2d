
package engine.game;

use engine.core.*;

interface ForceGenerator
{
	public abstract Vector2D getVelocity();
	public abstract void     setVelocity(Vector2D vel);
	
	public abstract void setImpulse(Vector2D impulse);
}
