
// Class : Process
// Abstract base class for the client and server processes

abstract class Process
{
	private native boolean create(string name);
	private native void setObject(Object self);
	
	public native void setScriptManager(ScriptManager manager);
	public native SceneGraph getSceneGraph();
	
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
