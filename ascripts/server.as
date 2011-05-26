
use bridge;
use player;
use scenegraph;
use inputforcegenerator;
use actionmap;
use bitstream;

class Server
{
	private boolean mStarted = false;
	private Player mPlayer;
	private BitStream mStream = new BitStream();
	
	public native Server();
	public native boolean create();
	public native void listen(int port);
	public native void update(real delta);
	public native SceneGraph getSceneGraph();
	public native void setActionMap(ActionMap map);
	public native void sendScriptEvent(BitStream stream, int client);

	private native boolean loadWorld(string filename, string name);
	
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
		
		Vector2D position = new Vector2D();
		position.setX(100.0);
		position.setY(30.0);
		
		setActionMap(new ActionMap());
		
		World world = getSceneGraph().getWorld();
		
		Creature controller = new Creature();
		controller.setPosition(position);
		controller.setName(player.getName());
		if ( !controller.create(world, "../objects/char.xml") )
		{
			// complain!
		}
		
		controller.setForceGenerator(new InputForceGenerator());
		
		player.setCreature(controller);
		
		createObjects(world);
		
		// notify the client
		mStream.clear();
		mStream.writeInt(1);
		mStream.writeInt(controller.getId());
		
		sendScriptEvent(mStream, player.getClient());
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
