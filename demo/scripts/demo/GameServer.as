
package demo;

use engine.collections.*;
use engine.core.*;
use engine.messages.*;
use engine.game.*;

class GameServer extends Server
{
	private boolean mStarted = false;
	private Hero   mHero;
	private Grudge mGrudge;
	private Entity mBox;
	
	public GameServer()
	{
		super();
	}
	
	public boolean create()
	{
		return listen(7000);
	}
	
	public int onClientConnecting()
	{
		if ( mStarted )
		{
			return Process.CONNECT_DENY_STARTED;
		}
		return Process.CONNECT_ALLOW;
	}
	
	public void onClientConnect(Player player)
	{
		start(player);
	}
	
	private void start(Player player)
	{
		addPlayer(player);
		
		if ( !createWorld("worlds/map1") )
		{
			// meh, something went terribly wrong..
		}
		
		mHero = createPlayer(player);
		player.setController(mHero);
		
		createObjects();
		
		mStarted = true;
	}
	
	private Hero createPlayer(Player player)
	{
		Vector2D position = new Vector2D();
		position.set(100, 30);
		
		ActionMap map = new DemoActionMap(this);
		
		InputController controller = new InputController();
		controller.setActionMap(map);
		
		Hero hero = (Hero)Entity.construct(this, "Hero", position, "objects/char");
		hero.setController(controller);
		hero.setForceGenerator(new InputForceGenerator());
		
		// notify the client
		ControllerMessage message = new ControllerMessage(hero.getId());
		sendMessage(player.getClientId(), message);
		
		return hero;
	}

	private void createObjects()
	{
		Vector2D pos = new Vector2D();
		
		/*
		pos.set(300, 00); Entity.construct(this, "box", pos, "objects/box");
		pos.set(300, 40); Entity.construct(this, "box", pos, "objects/box");
		pos.set(300, 80); Entity.construct(this, "box", pos, "objects/box");
		*/
		pos.set(727, 422);
		Entity.construct(this, "bridge", pos, "objects/bridge");
		
		/*
		Vector2D left = new Vector2D();
		left.set(727, 422);
		
		Vector2D right = new Vector2D();
		right.set(943, 422);
		
		Bridge bridge = new Bridge();
		bridge.create(this, left, right);
		
		Vector2D pos = new Vector2D();
		pos.set(190.0, 30);
		
		mGrudge = (Grudge) Actor.construct(this, "grudge1", pos, "../objects/grudge.xml");
		mGrudge.setLocalController(new AIController(this));
		*/
	}
}
