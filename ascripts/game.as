
use system.gamewindowfactory;
use system.gamewindow;

use client;
use server;

class Game
{
	private Server mServer;
	private Client mClient;
	
	public native GameWindowFactory getWindowFactory();
	public native void setActive(boolean active);
	
	public void initialize()
	{
		mServer = new Server();
		mServer.create();
		mServer.listen(7000);
		
		mClient = new Client();
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
