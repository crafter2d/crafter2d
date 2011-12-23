
use world;

class SceneGraph
{
	public native World getWorld();
	public native Creature find(int id);
	public native void setController(Creature controller);
}
