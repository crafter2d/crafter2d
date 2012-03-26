
use box2d.box2dbody;

class Actor extends Entity implements Collidable
{
	private InputForceGenerator mGenerator;
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
		if ( direction() )
		{
			return FACE_LEFT;
		}
		
		return FACE_RIGHT;
	}
	
	public void setFaceDirection(int direction)
	{
		if ( direction() && direction == FACE_RIGHT )
		{
			flip();
		}
		else if ( !direction() && direction == FACE_LEFT )
		{
			flip();
		}
	}
	
	// Operations
	
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
	public void updateAI(World world)
	{
	}
	
	// Natives
	
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
	public native void setController(Controller c);
}
