
package engine.game;

use engine.core.*;

class Entity implements Collidable
{
	private ForceGenerator mGenerator;
	private Controller mController;
	private AIState mState;
	private Body mBody;
	
	private int mDirection = FACE_RIGHT;
	private int mOnGround = 0;
	
	// static flags
	public static int FACE_LEFT  = 1;
	public static int FACE_RIGHT = 2;
	
	public native int getId();
	
	// Static interface
	
	public static Entity construct(Process process, string name, Vector2D position, string file)
	{
		ContentManager contentmgr = process.getContentManager();
	
		Entity object = (Entity)contentmgr.loadEntity(file);
		object.setPosition(position);
		object.setName(name);
		object.initialize();
				
		World world = process.getWorld();
		world.add(object);
		
		return object;
	}
	
	// - Public interface
	
	public void initialize()
	{
		QueryBodyComponentMessage msg = new QueryBodyComponentMessage();
		sendComponentMessage(msg);
		if ( msg.hasBody() )
		{
			mBody = msg.getBody();
		}
	}
	
	public void onCreated()
	{
	}
	
	// - Get/set
	
	public Vector2D getPosition()
	{
		return new Vector2D(getPositionX(), getPositionY());
	}
	
	public void setPosition(Vector2D pos)
	{
		setPosition(pos.x, pos.y);
	}
	
	public void setController(Controller c)
	{
		native(c);
		mController = c;
	}
	
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
	
	public Body getBody()
	{
		return mBody;
	}
	
	public void setBody(Body body)
	{
		mBody = body;
	}
	
	public AIState getState()
	{
		return mState;
	}
	
	public void setState(AIState state)
	{
		mState = state;
		mState.initialize(this);
	}
	
	public ForceGenerator getForceGenerator()
	{
		return mGenerator;
	}
	
	public void setForceGenerator(ForceGenerator generator)
	{
		mGenerator = generator;
		
		Body body = getBody();
		body.addForceGenerator(generator);
	}
	
	// - Query
	
	public boolean isOnGround()
	{
		return mOnGround > 0;
	}
	
	public boolean isLookingLeft()
	{
		return mDirection == FACE_LEFT;
	}
	
	public boolean isLookingRight()
	{
		return mDirection == FACE_RIGHT;
	}
	
	public boolean isLookingAt(Entity other)
	{
		Vector2D pos = getPosition();
		Vector2D otherpos = other.getPosition();
		
		if ( mDirection == FACE_RIGHT )
		{
			return pos.getX() < otherpos.getX();
		}
		
		return pos.getX() > otherpos.getX();
	}
	
	// - Operations
	
	public void lookAt(Entity other)
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
	
	public void jump()
	{
		if ( isOnGround() )
		{
			Vector2D vel = new Vector2D(0, -25.0);
			mGenerator.setImpulse(vel);
		}
	}
	
	// called from the AIController
	public void updateAI(Entity player)
	{
		if ( mState != null )
		{
			mState.perform(this, player);
		}
	}
	
	public void setAnimation(int index)
	{
		AnimationComponentMessage message = new AnimationComponentMessage();
		message.setAnimation(index);
		
		sendComponentMessage(message);
	}
	
	// - State
	
	public AttackState getAttackState()
	{
		return null;
	}
	
	// - Collision interface
	
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

	// - Natives
	
	private native real getPositionX();
	private native real getPositionY();
	private native void setPosition(real x, real y);
	
	public native void setName(string name);
	public native boolean hasLineOfSight(Entity to);
	public native void flip();
		
	protected native void sendComponentMessage(ComponentMessage message);
}
