
use creature;
use vector2d;

class ActionMap
{
	public native ActionMap();
	public native void setProcess(Server server);
	
	public void onKeyDown(Creature object, int action)
	{
		Vector2D velocity = object.getVelocity();
		
		if ( action == 3 ) // jump
		{
			object.jump();
		}
		else
		{
			switch ( action )
			{
				case 1: // move left
				{
					velocity.setX(-20.0);
					
					if ( object.direction() )
					{
						object.flip();
					}
					break;
				}
				case 2: // move right
				{
					velocity.setX(20.0);
					
					if ( !object.direction() )
					{
						object.flip();
					}
					break;
				}
			}
			
			object.setAnimation(2);
			object.getForceGenerator().setVelocity(velocity);  
			
			// <-- here the virtualobject creature created by the client should be found! 
			// otherwise the force generator will be non-existing...
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