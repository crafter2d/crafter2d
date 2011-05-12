
use creature;
use vector2d;

class ActionMap
{
	public native ActionMap();
	
	public void onKeyDown(Creature object, int action)
	{
		Vector2D velocity = object.getVelocity();
		
		if ( action == 3 ) // jump
		{
			object.jump();
		}
		else
		{
			if ( action == 1 ) // move left
			{
				velocity.setX(-20.0);
				
				if ( object.direction() )
				{
					object.flip();
				}
			}
			else if ( action == 2 ) // move right
			{
				velocity.setX(20.0);
				
				if ( !object.direction() )
				{
					object.flip();
				}
			}
			
			object.setAnimation(2);
			object.getForceGenerator().setVelocity(velocity);
		}
	}
	
	public void onKeyUp(Creature object, int action)
	{
		Vector2D velocity = object.getVelocity();
		
		if ( action == 1 || action == 2 )
		{
			velocity.setX(0.0);
		}
		
		object.setAnimation(1);
		object.getForceGenerator().setVelocity(velocity);
	}
}
