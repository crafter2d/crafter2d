
use engine.ui.*;
use engine.shapes.*;

use client;

class GameClient extends Client
{
	private Dialog mMessageBox;
	
	public GameClient()
	{
		super();
		
		mMessageBox = new Dialog("Message");
		mMessageBox.setBounds(new Rectangle(5, 5, 100, 40));
		
		Canvas canvas = Canvas.getInstance();
		canvas.add(mMessageBox);
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
