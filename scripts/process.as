
use engine.ui.*;
use engine.core.*;

// Class : Process
// Abstract base class for the client and server processes

abstract class Process
{
	private native boolean create(string name);
	private native void setObject(Object self);
	
	public native void setScriptManager(ScriptManager manager);
	public native Font getFont(string name, int size);
	public native Texture getTexture(string name);
	public native ContentManager getContentManager();
	public native World getWorld();
	public native void setWorld(World world);
	
	public boolean create()
	{		
		string classname = this.class.getName();
		boolean success = create(classname);
		if ( success )
		{
			// Let the process now this object so it can be used in
			// both the process space as in the game space.
			setObject(this);
		}
		return success;
	}
}
