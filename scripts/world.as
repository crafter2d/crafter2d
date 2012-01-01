
use box2d.box2dsimulator;

class World
{
	public native World()
	{
		super();
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
	public native SceneGraph getSceneGraph();
	public native void add(Creature creature);
	public native void setObjectLayer(int layer);
	public native void setFollowMode(int mode);
	public native void setFollowObject(Creature creature);
	public native void setFollowBorders(int left, int right, int top, int bottom);
	public native Box2DSimulator getSimulator();
}
