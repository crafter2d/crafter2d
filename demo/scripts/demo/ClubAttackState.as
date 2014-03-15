
package demo;

use engine.game.*;

class ClubAttackState extends AttackState
{
	public void initialize(Entity actor)
	{
		actor.setAnimation(3);
	}
   
	protected void attack(Entity actor, Entity player)
	{
		actor.lookAt(player);
	}
}
