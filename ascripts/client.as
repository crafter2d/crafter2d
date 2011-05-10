
use actionmap;
use bitstream;
use keymap;
use scenegraph;

class Client
{
	public native Client();
	public native boolean create();
	public native void connect(string host, int port, string name);
	public native void update(real delta);
	public native void render(real delta);
	public native SceneGraph getSceneGraph();
	public native void setActionMap(ActionMap map);
	public native void setKeyMap(KeyMap map);
	
	public void onConnected()
	{
		setActionMap(new ActionMap());
		
		KeyMap map = new KeyMap();
		map.bind(276, 1); 	// left
		map.bind(275, 2); 	// right
		map.bind(32, 3); 	// space -> jump
		setKeyMap(map);
	}
	
	public void onScriptEvent(BitStream stream)
	{
		int event = stream.readInt();
		
		if ( event == 1 ) // player controller
		{
			int controllerid = stream.readInt();
			
			SceneGraph graph = getSceneGraph();
			Creature creature = graph.find(controllerid);
			graph.setController(creature);
			
			World world = graph.getWorld();
			world.setFollowMode(0);
			world.setObjectLayer(0);
			world.setFollowObject(creature);
			world.setFollowBorders(150, 650, 100, 500);
		}
	}
	
	public void onWorldChanged()
	{
	}
}
