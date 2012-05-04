
abstract class AttackState extends AIState
{
	public void perform(Actor actor, Actor player)
	{
		attack(actor, player);
	}
	
	protected abstract void attack(Actor actor, Actor player);
}
