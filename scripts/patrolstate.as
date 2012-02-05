
class PatrolState extends AIState
{
	public AIState perform(Actor actor)
	{
		if ( actor.isOnGround() )
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
		
		return this;
	}
}
