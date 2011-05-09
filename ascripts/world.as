
use scenegraph;

class World
{
	public native World();
	
	public native string getName();
	public native SceneGraph getSceneGraph();
	public native void add(Creature creature);
}
