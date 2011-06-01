
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
		
		if ( !loadWorld("../worlds/map1.jwl", "World") )
		{
			// complain!
		}
		
		World world = getSceneGraph().getWorld();
		player.setCreature(createPlayer(player, world));
		
		createObjects(world);
		
		// notify the client
		mStream.clear();
		mStream.writeInt(1);
		mStream.writeInt(player.getCreature().getId());
		
		sendScriptEvent(mStream, player.getClient());
	}
	
	private Creature createPlayer(Player player, World world)
	{
		Vector2D position = new Vector2D();
		position.set(100, 30);
		
		ActionMap map = new ActionMap();
		map.setProcess(this);
		
		InputController controller = new InputController();
		controller.setActionMap(map);
		
		Creature creature = new Creature();
		creature.setPosition(position);
		creature.setName(player.getName());
		
		if ( !creature.create(world, "../objects/char.xml") )
		{
			throw new InvalidActorException();
		}
		
		creature.setController(controller);
		creature.setForceGenerator(new InputForceGenerator());
		
		return creature;
	}

	private void createObjects(World world)
	{
		Vector2D left = new Vector2D();
		left.set(727, 422);
		
		Vector2D right = new Vector2D();
		right.set(943, 422);
		
		Bridge bridge = new Bridge();
		bridge.create(world, left, right);
		
		left.set(180.0, 100.0);
		Creature box = new Creature();
		box.setPosition(left);
		box.setName("Box1");
		box.create(world, "../objects/box.xml");
		
		left.set(180.0, 60.0);
		box = new Creature();
		box.setPosition(left);
		box.setName("Box2");
		box.create(world, "../objects/box.xml");
		
		left.set(180.0, 40.0);
		box = new Creature();
		box.setPosition(left);
		box.setName("Box3");
		box.create(world, "../objects/box.xml");
		
		left.set(180.0, 20.0);
		box = new Creature();
		box.setPosition(left);
		box.setName("Box4");
		box.create(world, "../objects/box.xml");
	}
}
