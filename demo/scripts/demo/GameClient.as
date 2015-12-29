
package demo;

use engine.core.*;
use engine.input.*;
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
		map.bind(Key.LEFT, 1); 	// left (276)
		map.bind(Key.RIGHT, 2); 	// right (275)
		map.bind(KEY.SPACE, 3); 	// space -> jump (32)
		//map.bind(100, 6);   // leak detection
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

			Vector2D position = new Vector2D();
			position.set(200, 445);
			Entity fountain = Entity.construct(this, "Fountain", position, "objects/fountain");
					
			int controllerid = msg.getControllerId();
			Entity controller = (Entity) world.findEntity(controllerid);
			controller.initialize();
			controller.setForceGenerator(new InputForceGenerator());
			getPlayer().setController(controller);
			
			mLocalActionMap.setActor(controller);
			
			Viewport viewport = getViewport();
			int left = 150;
			int top = 100;
			int right = viewport.getWidth() - 150;
			int bottom = viewport.getHeight() - 100;
			
			world.setFollowMode(0);
			world.setObjectLayer(1);
			world.setFollowActor(controller);
			world.setFollowBorders(left, right, top, bottom);
		}
		else if ( message instanceof ActionMessage )
		{
			// perform the action on the given entity
			ActionMessage action = (ActionMessage) message;
			Entity entity = (Entity) getWorld().findEntity(action.getActorId());
			mLocalActionMap.onKeyDown(entity, action.getAction());
		}
	}
	
	public void onWorldChanged(World world)
	{
		super.onWorldChanged(world);
	}
}
