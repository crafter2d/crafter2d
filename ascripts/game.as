
use system.gamewindowfactory;
use system.gamewindow;

use gameclient;
use gameserver;

class Game
{
	private GameServer mServer;
	private GameClient mClient;
	
	public native GameWindowFactory getWindowFactory();
	public native void setActive(boolean active);
	
	public void initialize()
	{
		mServer = new GameServer();
		mServer.create();
		mServer.listen(7000);
		
		mClient = new GameClient();
		mClient.setWindow(getWindowFactory().createWindow());
		mClient.create();
		mClient.connect("localhost", 7000, "player");
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
