
package engine.game;

use system.*;
use engine.input.*;
use engine.core.*;

abstract class Client extends Process
{
	private MouseEventDispatcher 	mMouseDispatcher;
	private KeyEventDispatcher		mKeyDispatcher;
	private GameWindow 			mWindow;
		
	public Client()
	{
		super();
			
		mMouseDispatcher = new MouseEventDispatcher();
		mKeyDispatcher = new KeyEventDispatcher();
	}
	
	public native boolean connect(string host, int port);
	public native GameWindowFactory getWindowFactory();
	public native void update(real delta);
	public native void setKeyMap(KeyMap map);
	public native boolean isActive();
	public native Player getPlayer();
	public native void setPlayer(Player player);
	public native Viewport getViewport();
	public native void setOverlay(Renderable overlay);
	
	public boolean create()
	{
		if ( super.create() )
		{		
			return true;
		}
		return false;
	}
	
	// - Get/set
	
	public GameWindow getWindow()
	{
		return mWindow;
	}
	
	public void setWindow(GameWindow window)
	{
		mWindow = window;
		native;
	}
	
	// - Connection
	
	/// called by the engine when a connection has successfully been established
	/// player : the player object with information about the client
	public void onConnected(Player player)
	{
		addPlayer(player);
	}
	
	/// called by the engine when the connection has been denied by the server.
	/// reason : the reason given by the server
	public void onConnectionDenite(int reason)
	{
		string message = "Server denied the connection with unknown reason.";
		//MessageBox.showInformation(message);
	}
	
	/// called by the engine when another player joined the game.
	/// clientid : the ID of the player
	public void onPlayerJoined(int clientid)
	{
	}
	
	/// called by the engine when another player has left the game.
	/// clientid : the ID of the player
	public void onPlayerLeft(int clientid)
	{
	}
	
	/// called by the engine when the server notified that it's shutdown.
	public void onServerDown()
	{
	}
	
	/// called by the engine when a new world has been loaded
	/// after a server notification.
	public void onWorldChanged(World world)
	{
		setInternalWorld(world);
	}
	
	// - input handling
	
	private void onKeyEvent(int key, boolean pressed)
	{
		mKeyDispatcher.dispatch(key, pressed);
	}
	
	private void onMouseEvent(int x, int y, int buttons, int event)
	{
		mMouseDispatcher.dispatch(x, y, buttons, event);
	}
}
