
use creature;

class Player
{
	private Creature mController;
	
	public native string getName();
	public native int getClient();
	
	public native Creature getController();
	public native void setController(Creature creature);
	
	public void setCreature(Creature creature)
	{
		mController = creature;
		
		setController(creature);
	}
}
