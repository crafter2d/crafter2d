
package engine.game;

use system.*;
use engine.ui.*;
use engine.input.*;
use engine.core.*;

abstract class Client extends Process
{
	private EngineGraphics 			mGraphics = new EngineGraphics();
	private MouseEventDispatcher 	mMouseDispatcher;
	private KeyEventDispatcher		mKeyDispatcher;
	
	public native Client()
	{
		super();
		
		FontManager mgr = FontManager.getInstance();
		mgr.setProcess(this);
		
		TextureManager tmgr = TextureManager.getInstance();
		tmgr.setProcess(this);
		
		mMouseDispatcher = new MouseEventDispatcher();
		mKeyDispatcher = new KeyEventDispatcher();
	}
	
	private native void nativeRender(real delta);
	private native void nativeDisplay();
	private native void native_setWindow(GameWindow window);
	
	public native boolean connect(string host, int port, string name);
	public native void update(real delta);
	public native void setActionMap(ActionMap map);
	public native void setKeyMap(KeyMap map);
	public native boolean isActive();
	public native Player getPlayer();
	
	public boolean create()
	{
		if ( super.create() )
		{
			FontManager mgr = FontManager.getInstance();
			mgr.setProcess(this);
			
			return true;
		}
		return false;
	}
	
	public void setWindow(GameWindow window)
	{
		native_setWindow(window);
	}
	
	public EngineGraphics getGraphics()
	{
		return mGraphics;
	}
	
	public void render(real delta)
	{
		nativeRender(delta);
		
		Canvas canvas = Canvas.getInstance();
		canvas.paint(mGraphics);
		
		nativeDisplay();
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
		MessageBox.showInformation(message);
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