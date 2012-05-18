
package demo;

use engine.core.*;
use engine.ui.*;
use engine.messages.*;
use engine.collections.*;
use engine.game.*;

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
			MessageBox.showInformation("This is a messagebox!");
			
			return true;
		}
		
		return false;
	}
	
	public void onConnected(Player player)
	{
		addPlayer(player);
		
		setActionMap(new ActionMap());
		
		KeyMap map = new KeyMap();
		map.bind(276, 1); 	// left
		map.bind(275, 2); 	// right
		map.bind(32, 3); 	// space -> jump
		setKeyMap(map);
	}
	
	public void onConnectionDenite(int reason)
	{
		string message;
		switch ( reason )
		{
		case Process.CONNECT_DENY_STARTED:
			message = "Server has already started a game.";
			break;
		default:
			message = "Server denied the connection with unknown reason.";
			break;
		}
		
		MessageBox.showInformation(message);
	}
	
	protected void registerMessages(MessageMap messagemap)
	{
		//super.registerMessages(messagemap);
		
		messagemap.register(ControllerMessage.ID, ControllerMessage.class);
	}
	
	protected void onMessageReceived(Message message)
	{
		Map<int, Message> messagemap;
		
		if ( message instanceof ControllerMessage ) // player controller
		{
			ControllerMessage msg = (ControllerMessage)message;
			World world = getWorld();
			
			int controllerid = msg.getControllerId();
			Actor controller = (Actor) world.findEntity(controllerid);
			getPlayer().setController(controller);
			
			world.setFollowMode(0);
			world.setObjectLayer(0);
			world.setFollowActor(controller);
			world.setFollowBorders(150, 650, 100, 500);
		}
	}
	
	public void onWorldChanged()
	{
	}
}
