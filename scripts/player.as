
use creature;

class Player
{
	private Creature mController;
	
	public native string getName();
	public native int getClient();
	
	private native Creature getController();
	private native void setController(Creature creature);
	
	public Creature getCreature()
	{
		return mController;
	}
	
	public void setCreature(Creature creature)
	{
		mController = creature;
		
		setController(creature);
	}
}
