
use server;

class GameServer extends Server
{
	private boolean mStarted = false;
	private Player mPlayer;
	private BitStream mStream = new BitStream();
	
	public GameServer()
	{
		super();
	}
	
	public int onClientConnecting()
	{
		return 0;
	}
	
	public void onClientConnect(Player player)
	{
		start(player);
	}
	
	private void start(Player player)
	{
		mStarted = true;
		mPlayer = player;
		
		World world = getContentManager().load("../worlds/map1.jwl");
		if ( world == null )
		{
			// meh, something went terribly wrong..
		}
		
		setWorld(world);
		
		Actor controller = createPlayer(player);
		player.setController(controller);
		
		createObjects();
		
		// notify the client
		mStream.clear();
		mStream.writeInt(1);
		mStream.writeInt(controller.getId());
		
		sendScriptEvent(mStream, player.getClientId());
	}
	
	private Actor createPlayer(Player player)
	{
		Vector2D position = new Vector2D();
		position.set(100, 30);
		
		ActionMap map = new ActionMap();
		map.setProcess(this);
		
		InputController controller = new InputController();
		controller.setActionMap(map);
		
		Actor actor = Actor.construct(this, player.getName(), position, "../objects/char.xml");
		actor.setController(controller);
		actor.setForceGenerator(new InputForceGenerator());
		
		return actor;
	}

	private void createObjects()
	{
		Vector2D left = new Vector2D();
		left.set(727, 422);
		
		Vector2D right = new Vector2D();
		right.set(943, 422);
		
		Bridge bridge = new Bridge();
		bridge.create(this, left, right);
		
		left.set(180.0, 80.0);
		for ( int index = 0; index < 4; index++ )
		{
			Actor.construct(this, "Box" + index, left, "../objects/box.xml");
			
			left.setY(left.getY() - 25.0);
		}
	}
}
