
use box2d.box2dbody;

use vector2d;
use world;
use inputforcegenerator;

class Creature
{
	private InputForceGenerator mGenerator;
	private boolean mOnGround = false;
	
	public native Creature();
	public native boolean create(World parent, string file);
	public native int getId();
	public native Vector2D getPosition();
	public native void setPosition(Vector2D position);
	public native Vector2D getVelocity();
	public native void setVelocity(Vector2D vel);
	public native void setName(string name);
	public native void setAnimation(int index);
	public native boolean direction();
	public native void flip();
	public native Box2DBody getBody();
	
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
		return mOnGround;
	}
	
	public void setOnGround(boolean onground)
	{
		mOnGround = onground;
	}
	
	public void jump()
	{
		if ( mOnGround )
		{
			Vector2D vel = new Vector2D();
			vel.setY(-25.0);
			
			mGenerator.setImpulse(vel);
			mOnGround = false;
		}
	}
}
