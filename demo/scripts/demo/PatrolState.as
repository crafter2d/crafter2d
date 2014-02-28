
package demo;

use engine.core.*;
use engine.game.*;

class PatrolState extends AIState
{
   public void initialize(Entity entity)
   {
   }

	public void perform(Entity actor, Entity player)
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
	
	private boolean checkSight(Entity actor, Entity player)
	{
		return 	actor.isLookingAt(player) && 
				actor.getPosition().distance(player.getPosition()) < 100.0 && 
				actor.hasLineOfSight(player);
	}
	
	private void move(Entity actor)
	{
		Vector2D velocity = new Vector2D();
		if ( actor.getFaceDirection() == Entity.FACE_LEFT ) // to left
		{
			velocity.setX(-20.0);
		}
		else
		{
			velocity.setX(20.0);
		}
		
		actor.getForceGenerator().setVelocity(velocity);
	}
	
	private void attack(Entity actor, Entity player)
	{
		System.console.println("Moving attack state!");
		actor.getForceGenerator().setVelocity(new Vector2D());
		actor.lookAt(player);
		actor.setState(actor.getAttackState());
	}
}
