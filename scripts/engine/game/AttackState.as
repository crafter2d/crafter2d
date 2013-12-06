
package engine.game;

abstract class AttackState extends AIState
{
	public void perform(Entity actor, Entity player)
	{
		attack(actor, player);
	}
	
	protected abstract void attack(Entity actor, Entity player);
}
