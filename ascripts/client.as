
use system.*;

abstract class Client extends Process
{
	public native Client();
	
	public native void connect(string host, int port, string name);
	public native void update(real delta);
	public native void render(real delta);
	public native void setWindow(GameWindow window);
	public native void setActionMap(ActionMap map);
	public native void setKeyMap(KeyMap map);
	public native boolean isActive();
}
