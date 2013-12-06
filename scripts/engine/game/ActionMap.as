
package engine.game;

use engine.core.*;

abstract class ActionMap
{
	public native ActionMap();
	
	// - Calls
	
	public void onSwapLeakDetector(boolean down)
	{
		if ( down ) // execute only once
		{
			getProcess().swapLeakDetection();
		}
	}

	// Called by the engine when a key down action has been received
	public abstract void onKeyDown(Entity object, int action);
	
	// Called by the engine when a key up action has been received
	public abstract void onKeyUp(Entity object, int action);

	// - Natives
	
	public native Process getProcess();
	public native void setProcess(Process process);
	public native void bind(int action, string function);
}
