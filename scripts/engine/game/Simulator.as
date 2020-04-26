
package engine.game;

use box2d.*;
use engine.core.Vector2D;

abstract class Simulator
{
	public abstract boolean lineOfSight(Body from, Body to);
	
	public abstract void createRevoluteJoint(Box2DBody left, Box2DBody right, Vector2D anchor);
}
