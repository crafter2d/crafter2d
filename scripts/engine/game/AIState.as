
package engine.game;

abstract class AIState
{
   public abstract void initialize(Actor actor);
	public abstract void perform(Actor actor, Actor player);
}
