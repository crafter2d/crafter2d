
package engine.game;

use engine.core.*;

abstract class Simulator
{
	public abstract boolean lineOfSight(Body from, Body to);
	
	public abstract void createRevoluteJoint(Body left, Body right, Vector2D anchor);
}
