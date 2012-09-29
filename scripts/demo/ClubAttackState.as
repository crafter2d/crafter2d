
package demo;

use engine.game.*;

class ClubAttackState extends AttackState
{
   public void initialize(Actor actor)
   {
      actor.setAnimation(3);
   }
   
	protected void attack(Actor actor, Actor player)
	{
		actor.lookAt(player);
	}
}
