
use system.*;

use actionmap;
use bitstream;
use keymap;
use scenegraph;

abstract class Client
{
	public native Client();
	private native boolean create(string name);
	public native void connect(string host, int port, string name);
	public native void update(real delta);
	public native void render(real delta);
	public native SceneGraph getSceneGraph();
	public native void setWindow(GameWindow window);
	public native void setActionMap(ActionMap map);
	public native void setKeyMap(KeyMap map);
	public native boolean isActive();
	
	public boolean create()
	{
		// client creates a copy of this class in it's own format
		return create(this.class.getName());
	}
}
