
use system.gamewindowfactory;
use system.gamewindow;

use gameclient;
use gameserver;

class Game
{
	private GameServer mServer;
	private GameClient mClient;
	
	public native ScriptManager getScriptManager();
	public native GameWindowFactory getWindowFactory();
	public native void setActive(boolean active);
	
	public boolean initialize()
	{
		return createServer() && createClient();
	}
   
	public void shutdown()
	{
		mClient.destroy();
		mServer.destroy();
	}
   
	public void run(real delta)
	{
		mServer.update(delta);
		
		mClient.update(delta);
		mClient.render(delta);
		
		if ( !mClient.isActive() )
		{
			setActive(false);
		}
	}
	
	private boolean createServer()
	{
		mServer = new GameServer();
		mServer.setScriptManager(getScriptManager().spawnChild());
		if ( !mServer.create() )
		{
			return false;
		}
		
		return mServer.listen(7000);
	}
	
	private boolean createClient()
	{
		mClient = new GameClient();
		mClient.setScriptManager(getScriptManager().spawnChild());
		mClient.setWindow(getWindowFactory().createWindow());
		if ( !mClient.create() )
		{
			return false;
		}
		
		return mClient.connect("localhost", 7000, "player");
	}
}
