
use player;
use scenegraph;
use inputforcegenerator;

class Server
{
	private boolean mStarted = false;
	
	public native Server();
	public native boolean create();
	public native void listen(int port);
	public native void update(real delta);
	public native SceneGraph getSceneGraph();

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
		
		if ( !loadWorld("../worlds/map1.jwl", "World") )
		{
			// complain!
		}
		
		Vector2D position = new Vector2D();
		position.setX(100.0);
		position.setY(30.0);
		
		World world = getSceneGraph().getWorld();
		
		Creature controller = new Creature();
		controller.setPosition(position);
		controller.setName(player.getName());
		if ( !controller.create(world, "../objects/char.xml") )
		{
			// complain!
		}
		
		controller.setForceGenerator(new InputForceGenerator());
		
		player.setController(controller);
	}
}
