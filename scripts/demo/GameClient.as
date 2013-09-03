
package demo;

use engine.core.*;
// use engine.ui.*;
use engine.messages.*;
use engine.collections.*;
use engine.game.*;

class GameClient extends Client
{
	private DemoClientActionMap mLocalActionMap;
	
	public GameClient()
	{
		super();
	}
	
	public boolean create()
	{
		GameWindow window = getWindowFactory().createWindow();
		if ( !window.create("GameWindow", 800, 600, 32, false) )
		{
			return false;
		}
		setWindow(window);
	
		//MessageBox.showInformation("This is a messagebox!");
			
		return connect("localhost", 7000);
	}
	
	public void onConnected(Player player)
	{
		addPlayer(player);
		
		mLocalActionMap = new DemoClientActionMap();
		setActionMap(mLocalActionMap);
		
		KeyMap map = new KeyMap();
		map.bind(203, 1); 	// left (276)
		map.bind(205, 2); 	// right (275)
		map.bind(57, 3); 	// space -> jump (32)
		map.bind(100, 6);   // leak detection
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
		
		// MessageBox.showInformation(message);
	}
	
	protected void registerMessages(MessageMap messagemap)
	{
		//super.registerMessages(messagemap);
		
		messagemap.register(ControllerMessage.ID, ControllerMessage.class);
		messagemap.register(ActionMessage.ID, ActionMessage.class);
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
			controller.initialize();
			controller.setForceGenerator(new InputForceGenerator());
			getPlayer().setController(controller);
			
			mLocalActionMap.setActor(controller);
			
			world.setFollowMode(0);
			world.setObjectLayer(0);
			world.setFollowActor(controller);
			world.setFollowBorders(150, 650, 100, 500);
		}
		else if ( message instanceof ActionMessage )
		{
			// perform the action on the given actor
			ActionMessage action = (ActionMessage) message;
			Actor actor = (Actor) getWorld().findEntity(action.getActorId());
			mLocalActionMap.onKeyDown(actor, action.getAction());
		}
	}
}
