
package engine.game;

class Player
{
	public native int getClientId();
	
	public native Entity getController();
	public native void setController(Entity actor);
}
