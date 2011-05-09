
use creature;

class Player
{
	public native string getName();
	
	public native Creature getController();
	public native void setController(Creature creature);
}
