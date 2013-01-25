
package engine.game;

class Player
{
	public native int getClientId();
	
	public native Actor getController();
	public native void setController(Actor actor);
}
