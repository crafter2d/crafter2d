
use box2d.box2dbody;

use vector2d;
use world;
use inputforcegenerator;

class Creature
{
	private InputForceGenerator mGenerator;
	
	public native Creature();
	public native boolean create(World parent, string file);
	public native void setPosition(Vector2D position);
	public native void setName(string name);
	public native void setAnimation(int index);
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
}
