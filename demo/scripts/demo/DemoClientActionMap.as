
package demo;

use engine.core.*;
use engine.input.*;
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
		//bind(6, "onSwapLeakDetector");
		bind(1, "walkLeft");
		bind(2, "walkRight");
		bind(3, "jump");
	}
	
	public void walkLeft(boolean down)
	{
		InputForceGenerator force = (InputForceGenerator) mEntity.getForceGenerator();
		Vector2D velocity = force.getVelocity();
		
		if ( down )
		{
			velocity.setX(-128.0);
			mEntity.setAnimation(1);
			mEntity.setFaceDirection(Entity.FACE_LEFT);
		}
		else
		{
			velocity.setX(0.0);
			mEntity.setAnimation(0);
		}
		
		force.setVelocity(velocity);
	}
	
	public void walkRight(boolean down)
	{
		InputForceGenerator force = (InputForceGenerator) mEntity.getForceGenerator();
		Vector2D velocity = force.getVelocity();
		
		if ( down )
		{
			velocity.setX(128.0);
			mEntity.setAnimation(1);
			mEntity.setFaceDirection(Entity.FACE_RIGHT);
		}
		else
		{
			velocity.setX(0.0);
			mEntity.setAnimation(0);
		}
			
		force.setVelocity(velocity);
	}

	public void jump(boolean down)
	{
		if ( down )
		{
			mEntity.jump();
		}
	}
}
