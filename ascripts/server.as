
use system.*;

use bridge;
use player;
use scenegraph;
use inputforcegenerator;
use actionmap;
use bitstream;
use inputcontroller;
use invalidactorexception;

abstract class Server
{	
	public native Server()
	{
		super();
	}
	
	public native boolean create(string name);
	public native void listen(int port);
	public native void update(real delta);
	public native SceneGraph getSceneGraph();
	public native void sendScriptEvent(BitStream stream, int client);

	protected native boolean loadWorld(string filename, string name);
	
	public boolean create()
	{
		string classname = this.class.getName();
		return create(classname);
	}
}
