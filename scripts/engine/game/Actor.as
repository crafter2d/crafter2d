
package engine.game;

use box2d.box2dbody;

class Actor extends Entity implements Collidable
{
	private InputForceGenerator mGenerator;
	private Controller mController;
	private AIState mState;
	
	private int mDirection = FACE_LEFT;
	private int mOnGround = 0;
	
	// Static interface
	
	public static Actor construct(Process process, string name, Vector2D position, string file)
	{
		ContentManager contentmgr = process.getContentManager();
	
		Actor object = (Actor)contentmgr.loadEntity(file);
		object.setPosition(position);
		object.setName(name);
		
		World world = process.getWorld();
		world.add(object);
		
		return object;
	}
	
	public static int FACE_LEFT  = 1;
	public static int FACE_RIGHT = 2;
	
	// Get/set
	
	public int getFaceDirection()
	{
		return mDirection;
	}
	
	public void setInitialFaceDirection(int direction)
	{
		mDirection = direction;
	}
	
	public void setFaceDirection(int direction)
	{
		if ( mDirection != direction )
		{
			mDirection = direction;
			flip();
		}
	}
	
	public void setLocalController(Controller c)
	{
		setController(c);
		
		mController = c;
	}
	
	public AIState getState()
	{
		return mState;
	}
	
	public void setState(AIState state)
	{
		mState = state;
	}
	
	// Operations
	
	public boolean isLookingLeft()
	{
		return mDirection == FACE_LEFT;
	}
	
	public boolean isLookingRight()
	{
		return mDirection == FACE_RIGHT;
	}
	
	public boolean isLookingAt(Actor other)
	{
		Vector2D pos = getPosition();
		Vector2D otherpos = other.getPosition();
		
		if ( mDirection == FACE_RIGHT )
		{
			return pos.getX() < otherpos.getX();
		}
		
		return pos.getX() > otherpos.getX();
	}
	
	public void lookAt(Actor other)
	{
		Vector2D pos = getPosition();
		Vector2D otherpos = other.getPosition();
		
		if ( pos.getX() > otherpos.getX() )
		{
			setFaceDirection(FACE_LEFT);
		}
		else
		{
			setFaceDirection(FACE_RIGHT);
		}
	}
	
	public void turnAround()
	{
		if ( mDirection == FACE_LEFT )
		{
			mDirection = FACE_RIGHT;
		}
		else
		{
			mDirection = FACE_LEFT;
		}
		flip();
	}
	
	public InputForceGenerator getForceGenerator()
	{
		return mGenerator;
	}
	
	public void setForceGenerator(InputForceGenerator generator)
	{
		mGenerator = generator;
		
		Box2DBody body = getBody();
		body.addForceGenerator(generator);
		body.generateSensors();
	}
	
	public boolean isOnGround()
	{
		return mOnGround > 0;
	}
	
	public void collide(Object target, int side, boolean begin)
	{
		if ( side == 1 ) // bottom sensor
		{
			if ( begin )
			{
				mOnGround++;
			}
			else
			{
				mOnGround--;
			}
		}
	}
	
	public void jump()
	{
		if ( isOnGround() )
		{
			Vector2D vel = new Vector2D();
			vel.setY(-25.0);
			
			mGenerator.setImpulse(vel);
		}
	}
	
	// called from the AIController
	public void updateAI(Actor player)
	{
		if ( mState != null )
		{
			mState.perform(this, player);
		}
	}
	
	// - State
	
	public AttackState getAttackState()
	{
		return null;
	}
	
	// - Natives
	
	private native Actor();
	
	public native Vector2D getPosition();
	public native void setPosition(Vector2D position);
	public native Vector2D getVelocity();
	public native void setVelocity(Vector2D vel);
	public native void setName(string name);
	public native void setAnimation(int index);
	public native boolean direction();
	public native void flip();
	public native Box2DBody getBody();
	public native boolean hasLineOfSight(Actor to);
	
	private native void setController(Controller c);
}
