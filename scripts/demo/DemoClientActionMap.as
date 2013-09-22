
package demo;

use engine.core.*;
use engine.game.*;

class DemoClientActionMap extends ActionMap
{
	private Actor mEntity;
	
	public void setActor(Actor actor)
	{
		mEntity = actor;
	}
	
	public DemoClientActionMap()
	{
		bind(6, "onSwapLeakDetector");
		bind(1, "walkLeft");
		bind(2, "walkRight");
		bind(3, "jump");
	}
	
	public void walkLeft(boolean down)
	{
		real speed = 0.0;
		if ( down )
		{
			speed = -20.0;
			mEntity.setAnimation(1);
		}
		else
		{
			mEntity.setAnimation(0);
		}
	
		InputForceGenerator force = (InputForceGenerator) mEntity.getForceGenerator();
		Vector2D velocity = force.getVelocity();
		velocity.setX(speed);
		force.setVelocity(velocity);
		
		if ( mEntity.direction() )
		{
			mEntity.flip();
		}
	}
	
	public void walkRight(boolean down)
	{
		real speed = 0.0;
		if ( down )
		{
			speed = 20.0;
			mEntity.setAnimation(1);
		}
		else
		{
			mEntity.setAnimation(0);
		}
			
		InputForceGenerator force = (InputForceGenerator) mEntity.getForceGenerator();
		Vector2D velocity = force.getVelocity();
		velocity.setX(speed);
		force.setVelocity(velocity);
		
		if ( !mEntity.direction() )
		{
			mEntity.flip();
		}
	}

	public void jump(boolean down)
	{
		if ( down )
		{
			mEntity.jump();
		}
	}

	public void onKeyDown(Actor object, int action)
	{
		if ( action == 3 )
		{
			object.jump();
		}
		else
		{
			InputForceGenerator force = (InputForceGenerator) object.getForceGenerator();
			Vector2D velocity = force.getVelocity();
			
			switch ( action )
			{
				case 1:
					velocity.setX(-20.0);
						
					if ( object.direction() )
					{
						object.flip();
					}
					break;
				case 2:
					velocity.setX(20.0);
						
					if ( !object.direction() )
					{
						object.flip();
					}
					break;
			}
			
			force.setVelocity(velocity);
		}
	}

	public void onKeyUp(Actor object, int action)
	{
	}
}
