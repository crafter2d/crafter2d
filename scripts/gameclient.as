
use engine.ui.*;

class GameClient extends Client
{
	public GameClient()
	{
		super();
	}
	
	public boolean create()
	{
		if ( super.create() )
		{			
			// MessageBox.showInformation("This is a messagebox!");
			
			return true;
		}
		
		return false;
	}
	
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
			World world = getWorld();
			
			int controllerid = stream.readInt();
			Actor controller = (Actor) world.findEntity(controllerid);
			getPlayer().setController(controller);
			
			world.setFollowMode(0);
			world.setObjectLayer(0);
			world.setFollowObject(controller);
			world.setFollowBorders(150, 650, 100, 500);
		}
	}
	
	public void onWorldChanged()
	{
	}
}
