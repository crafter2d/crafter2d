
use box2d.box2dbody;

// grudge enemy

class Grudge extends Actor
{
	private AIState mState;

	public void onCreated()
	{
		mState = new PatrolState();
	
		setForceGenerator(new InputForceGenerator());
	}

	public void collide(Object target, int side, boolean begin)
	{
		super.collide(target, side, begin);
		
		if ( target instanceof Bound )
		{
			if ( (side == 2 && direction()) || (side == 3 && !direction()) )
			{
				flip();
			}
			
			if ( isOnGround() )
			{
				// when on the ground, start the walking animation
				setAnimation(2);
			}
		}
	}
	
	// called from the AIController
	public void updateAI(World world)
	{
		mState = mState.perform(this, null); // world.getFollowActor());
	}
}
