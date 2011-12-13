
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
		mServer = new GameServer();
		mServer.setScriptManager(getScriptManager().spawnChild());
		if ( !mServer.create() )
		{
			return false;
		}
		mServer.listen(7000);
		
		mClient = new GameClient();
		mClient.setScriptManager(getScriptManager().spawnChild());
		mClient.setWindow(getWindowFactory().createWindow());
		if ( !mClient.create() )
		{
			return false;
		}
		mClient.connect("localhost", 7000, "player");
		
		return true;
	}
   
	public void shutdown()
	{
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
}
