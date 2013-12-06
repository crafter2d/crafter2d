
package demo;

use engine.core.*;
use engine.game.*;

class DemoClientActionMap extends ActionMap
{
	private Entity mEntity;
	
	public void setActor(Entity entity)
	{
		mEntity = entity;
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
			speed = -100.0;
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
			speed = 100.0;
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
}
