
package engine.game;

use box2d.box2dsimulator;
use engine.collections.*;

class World
{
	private ArrayList<Actor> mActors = new ArrayList<Actor>();
	
	public native World()
	{
		super();
	}
	
	// Object registration
	
	public void onEntityAdded(Entity entity)
	{
		if ( entity instanceof Actor )
		{
			// mActors.add((Actor)entity);
		}
	}
	
	// Collision detection
	
	public void onObjectCollision(Object source, Object target, int side, boolean begin)
	{
		if ( source instanceof Collidable )
		{
			Collidable col = (Collidable) source;
			col.collide(target, side, begin);
		}
	}
	
	// Natives
	
	public native string getName();
	public native void add(Actor actor);
	public native Actor findEntity(int id);
	public native void setObjectLayer(int layer);
	public native void setFollowMode(int mode);
	public native Actor getFollowActor();
	public native void setFollowActor(Actor actor);
	public native void setFollowBorders(int left, int right, int top, int bottom);
	public native Box2DSimulator getSimulator();
}
