
package demo;

use engine.core.*;
use engine.game.*;

class PatrolState extends AIState
{
   public void initialize(Actor actor)
   {
   }

	public void perform(Actor actor, Actor player)
	{
		if ( actor.isOnGround() )
		{
			if ( checkSight(actor, player) )
			{
				attack(actor, player);
			}
			else
			{
				move(actor);
			}
		}
	}
	
	private boolean checkSight(Actor actor, Actor player)
	{
		return 	actor.isLookingAt(player) && 
				actor.getPosition().distance(player.getPosition()) < 100.0 && 
				actor.hasLineOfSight(player);
	}
	
	private void move(Actor actor)
	{
		Vector2D velocity = new Vector2D();
		if ( actor.getFaceDirection() == Actor.FACE_LEFT ) // to left
		{
			velocity.setX(-20.0);
		}
		else
		{
			velocity.setX(20.0);
		}
		
		actor.getForceGenerator().setVelocity(velocity);
	}
	
	private void attack(Actor actor, Actor player)
	{
		System.console.println("Moving attack state!");
		actor.getForceGenerator().setVelocity(new Vector2D());
		actor.lookAt(player);
		actor.setState(actor.getAttackState());
	}
}
