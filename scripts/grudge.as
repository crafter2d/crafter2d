
use box2d.box2dbody;

// grudge enemy

class Grudge extends Actor
{
	public void onCreated()
	{
		Box2DBody body = getBody();
		body.generateSensors();
	}

	public void collide(Object target, int side, boolean begin)
	{
		super.collide(target, side, begin);
		
		if ( target instanceof Bound )
		{
			if ( side == 2 || side == 3 )
			{
				flip();
			}
		}
	}
	
	public void updateAI()
	{
	}
}
