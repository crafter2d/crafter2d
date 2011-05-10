
use box2d.box2dbody;

use vector2d;
use world;
use inputforcegenerator;

class Creature
{
	private InputForceGenerator mGenerator;
	private int mOnGround = 0;
	
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
		
		getBody().addForceGenerator(generator);
		getBody().generateSensors();
	}
	
	public boolean isOnGround()
	{
		return mOnGround > 0;
	}
	
	public void increaseOnGround()
	{
		mOnGround++;
	}
	
	public void decreaseOnGround()
	{
		mOnGround--;
	}
}
