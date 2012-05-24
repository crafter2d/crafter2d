
package engine.game;

class Player
{
	private Actor mController;
	
	public Actor getController()
	{
		return mController;
	}
	
	public void setController(Actor actor)
	{
		mController = actor;
		
		native_setController(actor);
	}
	
	// Natives
	
	public native int getClientId();
	
	private native Actor native_getController();
	private native void native_setController(Actor actor);
}
