
package demo;

use engine.core.*;
use engine.input.*;
use engine.messages.*;
use engine.collections.*;
use engine.game.*;
use ui.*;

class GameClient extends Client
{		
	private DemoClientActionMap mLocalActionMap;
	private YuiSystem mYuiSystem;
	private YuiWindow mWindow;
	
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
		initUI();
		
		Player player = new Player();
		setPlayer(player);
		addPlayer(player);
		
		if ( !createWorld("worlds/map1") )
		{
			// meh, something went terribly wrong..
			return false;
		}
		
		Vector2D position = new Vector2D();
		position.set(100, 630);
		
		Hero hero = (Hero)Entity.construct(this, "Hero", position, "objects/char");
		hero.setForceGenerator(new InputForceGenerator());
		
		position.set(850, 300);
		Mushroom mush = (Mushroom) Entity.construct(this, "mushroom", position, "objects/mushroom");
		
		/*
		position.set(40, 755);
		Entity.construct(this, "sign1", position, "objects/sign2");
		position.set(150, 630);
		Entity.construct(this, "tree1", position, "objects/tree2");
		position.set(1500, 500);
		Entity.construct(this, "tree2", position, "objects/tree2");
		position.set(550, 515);
		Entity.construct(this, "tree4", position, "objects/tree1");
		position.set(1300, 540);
		Entity e = Entity.construct(this, "tree3", position, "objects/tree3");
		e.scale(0.8);
		position.set(280, 765);
		Entity.construct(this, "stone1", position, "objects/stone");
		position.set(320, 765);
		Entity.construct(this, "bush1", position, "objects/bush1");
		position.set(1400, 630);
		Entity.construct(this, "bush2", position, "objects/bush3");
		*/
		
		Viewport viewport = getViewport();
		int left = 150;
		int top = 100;
		int right = viewport.getWidth() - 150;
		int bottom = viewport.getHeight() - 100;

		World world = getWorld();
		world.setFollowMode(0);
		world.setObjectLayer(1);
		world.setFollowActor(hero);
		world.setFollowBorders(left, right, top, bottom);
		
		initKeymap(hero);
		
		return true; // connect("localhost", 7000);
	}
	
	public void onConnected(Player player)
	{
		addPlayer(player);
		
		mLocalActionMap = new DemoClientActionMap();
		setActionMap(mLocalActionMap);
		
		KeyMap map = new KeyMap();
		map.bind(Key.LEFT, 1); 	// left (276)
		map.bind(Key.RIGHT, 2); 	// right (275)
		map.bind(Key.SPACE, 3); 	// space -> jump (32)
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
	
	private void initKeymap(Hero hero)
	{
		mLocalActionMap = new DemoClientActionMap();
		mLocalActionMap.setActor(hero);
		setActionMap(mLocalActionMap);
		
		KeyMap map = new KeyMap();
		map.bind(Key.LEFT, 1); 	// left (276)
		map.bind(Key.RIGHT, 2); 	// right (275)
		map.bind(Key.SPACE, 3); 	// space -> jump (32)
		//map.bind(100, 6);   // leak detection
		setKeyMap(map);
	}
	
	private boolean initUI()
	{
		GameWindow window = getWindow();
		
		mYuiSystem = new YuiSystem();
		if ( !mYuiSystem.initialize(getContentManager(), getScriptManager(), window.getWidth(), window.getHeight()) )
		{
			return false;
		}
		mYuiSystem.setTheme("ui/theme");
		setOverlay(mYuiSystem);
		
		mWindow = mYuiSystem.load("ui/load");
		
		return true;
	}
}
