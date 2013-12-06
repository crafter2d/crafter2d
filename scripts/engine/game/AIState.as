
package engine.game;

abstract class AIState
{
	public abstract void initialize(Entity actor);
	public abstract void perform(Entity actor, Entity player);
}
