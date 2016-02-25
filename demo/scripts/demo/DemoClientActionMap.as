
package demo;

use engine.core.*;
use engine.input.*;
use engine.game.*;

class DemoClientActionMap extends ActionMap
{
	private Entity mEntity;
	private real mMass;
	private real mLastJump = 0.0;
	
	private int ANIM_IDLE = 0;
	private int ANIM_WALK = 1;
	private int ANIM_RUN = 2;
	private int ANIM_JUMP = 3;
	
	public void setActor(Entity entity)
	{
		mEntity = entity;
		mMass = entity.getBody().getMass();
	}
	
	public DemoClientActionMap()
	{
		//bind(6, "onSwapLeakDetector");
		bind(1, "walkLeft");
		bind(2, "walkRight");
		bind(3, "jump");
	}
	
	public void walkLeft(boolean down, real delta)
	{
		Vector2D velocity = mEntity.getBody().getLinearVelocity();
		real desiredvel = 0.0;
		
		if ( down )
		{
			desiredvel = -10.0;
			mEntity.setAnimation(ANIM_RUN);
			mEntity.setFaceDirection(Entity.FACE_LEFT);
		}
		else
		{
			mEntity.setAnimation(ANIM_IDLE);
		}
		
		real velchange = desiredvel - velocity.x;
		real force = mMass * velchange;
		velocity.set(force, 0.0);
		
		mEntity.getForceGenerator().setImpulse(velocity);
	}
	
	public void walkRight(boolean down, real delta)
	{
		Vector2D velocity = mEntity.getBody().getLinearVelocity();
		real desiredvel = 0.0;
		
		if ( down )
		{
			desiredvel = 10.0;
			mEntity.setAnimation(ANIM_RUN);
			mEntity.setFaceDirection(Entity.FACE_RIGHT);
		}
		else
		{
			mEntity.setAnimation(ANIM_IDLE);
		}
			
		real velchange = desiredvel - velocity.x;
		real force = mMass * velchange;
		velocity.set(force, 0.0);
		
		mEntity.getForceGenerator().setImpulse(velocity);
	}

	public void jump(boolean down, real delta)
	{
		mLastJump = mLastJump - delta;
		if ( down && mLastJump <= 0.0 )
		{
			mLastJump = 0.15;
			mEntity.jump();
		}
	}
}
