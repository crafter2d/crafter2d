
package demo;

use engine.game.*;

class ClubAttackState extends AttackState
{
	protected void attack(Actor actor, Actor player)
	{
		actor.setAnimation(3);
		
		actor.lookAt(player);
	}
}
