
class PatrolState extends AIState
{
	public AIState perform(Actor actor, Actor player)
	{
		if ( actor.isOnGround() )
		{
			if ( checkSight(actor, player) )
			{
				// go 
			}
		
			move(actor);
		}
		
		return this;
	}
	
	private boolean checkSight(Actor actor, Actor player)
	{
		return false;
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
}
