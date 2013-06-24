
package engine.game;

abstract class Entity
{
	public native int getId();
	
	public void onCreated()
	{
	}
	
	protected native void sendComponentMessage(ComponentMessage message);
}
